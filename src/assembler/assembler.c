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

int assembly (int* machine_code, struct metka_t* metkas, const char* filename)
{
    FILE* people_code = fopen(filename, "r"); // NOTE rb
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

        printf (">>> " BLUE_TEXT("[%02d = 0x%04x]") PURPLE_TEXT("cmd")" = "YELLOW_TEXT("'%s'")"\n", count_itr, (uint)count_itr, cmd);

        if (strchr(cmd, ':') != NULL)
        {
            int num = 0;
            sscanf(cmd, "%d", &num);

            printf (">>> >>> BLYAAAAAA METKA DETKA '%d'\n", num);

            metkas[num].addr = count_itr;

            count_mtk++;

            printf (">>> LABELS:\n      ");
            for (int i = 0; i < MAX_SIZE; i++)
                if (metkas[i].addr) printf (BLUE_TEXT("[%d]:")"%d ", i, metkas[i].addr);
            printf ("\n    <<< (end)\n");
        }

        // TODO make function that returns enum by command name
        // TODO switch

        if (strcmp(cmd, "push") == 0)
        {
            machine_code[count_itr] = PUSH_CODE;
            count_itr++;

            fscanf (people_code, "%s", cmd);

            compile_arg (cmd, machine_code, &count_itr);
        }
        else if (strcmp(cmd, "pop") == 0)
        {
            machine_code[count_itr] = POP_CODE;
            count_itr++;

            fscanf (people_code, "%s", cmd);

            compile_arg (cmd, machine_code, &count_itr);
        }
        else if (strcmp(cmd, "add") == 0)
        {
            machine_code[count_itr] = ADD_CODE;
            count_itr++;
        }
        else if (strcmp(cmd, "sub") == 0)
        {
            machine_code[count_itr] = SUB_CODE;
            count_itr++;
        }
        else if (strcmp(cmd, "mul") == 0)
        {
            machine_code[count_itr] = MUL_CODE;
            count_itr++;
        }
        else if (strcmp(cmd, "div") == 0)
        {
            machine_code[count_itr] = DIV_CODE;
            count_itr++;
        }
        else if (strcmp(cmd, "out") == 0)
        {
            machine_code[count_itr] = OUT_CODE;
            count_itr++;
        }
        else if (strcmp(cmd, "hlt") == 0)
        {
            machine_code[count_itr] = HLT_CODE;
            count_itr++;
        }
        else if (strcmp(cmd, "ja") == 0)
        {
            printf (">>> >>> GOT: ja\n");

            machine_code[count_itr] = JA_CODE;
            count_itr++;

            int num = 0;
            fscanf(people_code, "%d:", &num);

            printf (">>> >>> >>> num = %d\n", num);

            machine_code[count_itr] = metkas[num].addr;

            count_itr++;
        }
        else if (strcmp(cmd, "jb") == 0)
        {
            printf (">>> >>> GOT: jb\n");

            machine_code[count_itr] = JB_CODE;
            count_itr++;

            int num = 0;
            fscanf(people_code, "%d:", &num);

            printf (">>> >>> >>> num = %d\n", num);

            machine_code[count_itr] = metkas[num].addr; // TODO check if metka exists

            count_itr++;
        }
        else if (strcmp(cmd, "call") == 0)
        {
            machine_code[count_itr] = CALL_CODE;
            count_itr++;

            int num = 0;

            fscanf(people_code, "%d:", &num);

            printf (">>> >>> >>> num = %d\n", num);

            machine_code[count_itr] = metkas[num].addr;

            count_itr++;
        }
        else if (strcmp(cmd, "ret") == 0)
        {
            machine_code[count_itr] = RET_CODE;
            count_itr++;
        }
        else if (strcmp(cmd, "draw") == 0)
        {
            machine_code[count_itr] = DRAW_CODE;
            count_itr++;
        }
        else if (strcmp(cmd, "je") == 0)
        {
            printf (">>> >>> GOT: je\n");

            machine_code[count_itr] = JE_CODE;
            count_itr++;

            int num = 0;
            fscanf(people_code, "%d:", &num);

            printf (">>> >>> >>> num = %d\n", num);

            machine_code[count_itr] = metkas[num].addr;

            count_itr++;
        }
        else if (strcmp(cmd, "jne") == 0)
        {
            printf (">>> >>> GOT: je\n");

            machine_code[count_itr] = JNE_CODE;
            count_itr++;

            int num = 0;
            fscanf(people_code, "%d:", &num);

            printf (">>> >>> >>> num = %d\n", num);

            machine_code[count_itr] = metkas[num].addr;

            count_itr++;
        }
        else if (strcmp(cmd, "sqrt") == 0)
        {
            machine_code[count_itr] = SQRT_CODE;
            count_itr++;
        }
        else if (strcmp(cmd, "in") == 0)
        {
            machine_code[count_itr] = IN_CODE;
            count_itr++;
        }
        else if (strcmp(cmd, "out") == 0)
        {
            machine_code[count_itr] = OUT_CODE;
            count_itr++;
        }
        /*else
        {
            printf("\nERROR: unknown command\n");
            return -1;
        }*/ //FIXME
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
    printf(GREEN_TEXT("have_bracket")" = "YELLOW_TEXT("%d")", "GREEN_TEXT("have_plus")" = "YELLOW_TEXT("%d")"\n\n", have_bracket, have_plus);

    if (have_bracket && have_plus)
    {
        char str[MAX_SIZE] = {};
        int num = 0;

        int res = sscanf (cmd, "[%1[a-d]x + %d]", str, &num);
        assert (res == 2);

        machine_code[(*count)++] = MEM_ARG | REG_ARG | IMMED_ARG;
        machine_code[(*count)++] = num;
        machine_code[(*count)++] = str[0] - 'a' + 1; //< reg

        return 0;
    }

    // TODO else if (have_bracket) // have_bracket && !have_plus

    if (have_bracket && !have_plus)
    {
        char str[MAX_SIZE] = {};
        int num = 0;

        int res_1 = sscanf (cmd, "[%1[a-d]x]", str);
        int res_2 = sscanf (cmd, "[%d]", &num);

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

    if (!have_bracket && have_plus)
    {
        char str[MAX_SIZE] = {};
        int num = 0;

        int res = sscanf (cmd, "%1[a-d]x + %d", str, &num);
        assert (res == 2);

        machine_code[(*count)++] = REG_ARG | IMMED_ARG;
        machine_code[(*count)++] = num;
        machine_code[(*count)++] = str[0] - 'a' + 1;

        return 0;
    }

    if (!have_bracket && !have_plus)
    {
        char str[MAX_SIZE] = {};
        int num = 0;

        int res_1 = sscanf (cmd, "%1[a-d]x", str);
        int res_2 = sscanf (cmd, "%d", &num);

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
