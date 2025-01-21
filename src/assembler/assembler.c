
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "../include/color_print.h"
#include "../include/enum.h"
#include "assembler.h"
#include "struct.h"

int label_ctor (struct metka_t* metkas);

int label_dtor (struct metka_t* metkas);

int assembly (int* machine_code, struct metka_t* metkas, const char* filename);

int load_code (int* machine_code, int count, const char* filename);

int machine_code_dump (int count_itk, int* machine_code);

int compile_arg (char* cmd, int* machine_code, int* count);

int main (int argc, const char* argv[])
{
    const char* in_filename  = (argc >= 2)? argv[1] : "math_exmpl.asm";
    const char* out_filename = (argc >= 3)? argv[2] : "math_exmpl.bin";

    int      machine_code[MAX_ARR]  = {};
    struct metka_t metkas[MAX_SIZE] = {};

    for (int i = 0; i < MAX_SIZE; i++)
        metkas[i].addr = -1;

    int count_cmmnds = assembly (machine_code, metkas, in_filename);

    if (count_cmmnds == -1)
        return 1;

    machine_code_dump (count_cmmnds, machine_code);

    assembly (machine_code, metkas, in_filename);

    machine_code_dump (count_cmmnds, machine_code);

    if (load_code (machine_code, count_cmmnds, out_filename) == 1)
    {
        printf(RED_TEXT("ERROR")" in load_code, fwrite returns 0\n");
        return 1;
    }

    return 0;
}

#define DEF_CMD_(command, num, arg) else if (strcasecmp (cmd, #command) == 0)                                   \
                                    {                                                                           \
                                        machine_code[count_itr++] = command##_CODE;                             \
                                        fprintf (stderr, BLUE_TEXT("\n%d\n\n"), machine_code[count_itr - 1]);   \
                                                                                                                \
                                        if (arg == 1)                                                           \
                                        {                                                                       \
                                            fgets (cmd, MAX_ARR, people_code);                                  \
                                            compile_arg (cmd, machine_code, &count_itr);                        \
                                        }                                                                       \
                                                                                                                \
                                        if (arg == 2)                                                           \
                                        {                                                                       \
                                            int numb = 0;                                                       \
                                            fscanf(people_code, "%d:", &numb);                                  \
                                            fprintf(stderr, "%d\n", numb);                                      \
                                            machine_code[count_itr++] = metkas[numb].addr;                      \
                                        }                                                                       \
                                    }

int assembly(int* machine_code, struct metka_t* metkas, const char* filename)
{
    FILE* people_code = fopen(filename, "rb");
    if (people_code == NULL)
    {
        fprintf (stderr, RED_TEXT ("ERROR: ")"[people_code] = NULL, file %s: ", filename);
        perror ("");
        return -1;
    }

    int count_mtk = 0;
    int count_itr = 0;

    while (!feof(people_code))
    {
        char cmd[MAX_ARR] = {};

        if (fscanf(people_code, "%10s", cmd) != 1)
            break;

	if (cmd[0] == ';') // TODO strchr
	    continue;

        printf (">>> " BLUE_TEXT ("[%02d = 0x%04x]") PURPLE_TEXT ("cmd")" = "YELLOW_TEXT("'%s'")"\n", count_itr, (uint) count_itr, cmd);

        if (strchr(cmd, ':') != NULL)
        {
            int num = 0;
            sscanf(cmd, "%d", &num);

            fprintf (stderr, ">>> >>> METKA '%d'\n", num);

            metkas[num].addr = count_itr;

            count_mtk++;

            //fprintf (stderr, "\n>>> LABELS:\n      ");
            //for (int i = 0; i < MAX_SIZE; i++)
            //    if (metkas[i].addr) printf (BLUE_TEXT("[%d]:")"%d ", i, metkas[i].addr);
            //fprintf (stderr, "\n    <<< (end)\n");
        }
        #include "../include/commands.h"

    }

    printf("\n");

    fclose(people_code);

    return count_itr;
}

int load_code (int* machine_code, int count, const char* filename)
{
    printf(BLUE_TEXT("%s():") PURPLE_TEXT("count_cmmnds")" = "YELLOW_TEXT("%d")"\n", __FUNCTION__, count);

    struct header_t header = { SIGNATURE, VERSION, count };

    FILE* machine = fopen(filename, "wb");
    if (machine == NULL)
    {
        fprintf (stderr, RED_TEXT ("ERROR: ")"[machine] = NULL, filename %s: ", filename);
        perror ("");
        return -1;
    }

    if (fwrite (&header     , sizeof (header)         , 1            , machine) == 0)
        return 1;
    if (fwrite (machine_code, sizeof (machine_code[0]), (size_t)count, machine) == 0)
        return 1;

    fclose(machine);

    return 0;
}

int machine_code_dump (int count_itk, int* machine_code)
{
    printf (LIGHT_BLUE_TEXT("----------------------------------------------------------------------------------------------------------\n\n"));

    int ip = 0;

    printf (BLUE_TEXT("code:"));
    for (int i = 0; i < count_itk; i++)
        printf (BLUE_TEXT("  %02d "), i);

    printf("\n     ");

    for (int i = 0; i < count_itk; i++)
        printf (GREEN_TEXT("  %02d "), machine_code[i]);

    printf (PURPLE_TEXT("\n       %*s^ ip = %02d"), ip * 5, "", ip);

    printf (LIGHT_BLUE_TEXT("\n\n----------------------------------------------------------------------------------------------------------\n"));

    printf(LIGHT_BLUE_TEXT("\nPress enter...\n"));
    getchar ();

    return 0;
}

int compile_arg (char* cmd, int* machine_code, int* count)
{
    int have_bracket = (strchr (cmd, '[') != NULL);
    int have_plus    = (strchr (cmd, '+') != NULL);

    printf(GREEN_TEXT("have_bracket")" = "YELLOW_TEXT("%d")", "GREEN_TEXT("have_plus")" = "YELLOW_TEXT("%d")"\n", have_bracket, have_plus);

    if (have_bracket && have_plus)
    {
        char str[MAX_SIZE] = {};
        int num = 0;

        int res = sscanf (cmd, " [%1[a-d]x + %d] ", str, &num);

        assert (res == 2);

        machine_code[(*count)++] = MEM_ARG | REG_ARG | IMMED_ARG;
        machine_code[(*count)++] = num;
        machine_code[(*count)++] = str[0] - 'a' + 1; // <- reg

        return 0;
    }

    else if (have_bracket && !have_plus)
    {
        char str[MAX_SIZE] = {};
        int num = 0;

        int res_1 = sscanf (cmd, " [%1[a-d]x]", str);
        int res_2 = sscanf (cmd, " [%d]", &num);

        if (res_1 == 1)
        {
            machine_code[(*count)++] = MEM_ARG | REG_ARG;
            machine_code[(*count)++] = str[0] - 'a' + 1;
        }

        if (res_2 == 1)
        {
            machine_code[(*count)++] = MEM_ARG | IMMED_ARG;
            machine_code[(*count)++] = num;
        }

        return 0;
    }

    else if (!have_bracket && have_plus)
    {
        char str[MAX_SIZE] = {};
        int num = 0;

        int res = sscanf (cmd, " %1[a-d]x + %d", str, &num);
        assert (res == 2);

        machine_code[(*count)++] = REG_ARG | IMMED_ARG;
        machine_code[(*count)++] = num;
        machine_code[(*count)++] = str[0] - 'a' + 1;

        return 0;
    }

    else if (!have_bracket && !have_plus)
    {
        char str[MAX_SIZE] = {};
        int num = 0;

        int res_1 = sscanf (cmd, " %1[a-d]x", str);
        int res_2 = sscanf (cmd, " %d", &num);

        if (res_1 == 1)
        {
            machine_code[(*count)++] = REG_ARG;
            machine_code[(*count)++] = str[0] - 'a' + 1;
        }

        if (res_2 == 1)
        {
            machine_code[(*count)++] = IMMED_ARG;
            machine_code[(*count)++] = num;
        }

        return 0;
    }

    return 1;
}
