#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "color_print.h"

#define MAX_SIZE 30
#define VERSION  2
#define MAX_ARR  100

struct head
{
    uint32_t sig;
    int      ver;
    int      size;
};

struct mtk
{
    char  name_mtk[MAX_SIZE];
    int   addr;
};

enum operartion_code
{
    PUSH_CODE  =  1,
    POP_CODE   =  2,
    ADD_CODE   =  3,
    SUB_CODE   =  4,
    MUL_CODE   =  5,
    DIV_CODE   =  6,
    OUT_CODE   =  7,
    HLT_CODE   =  8,
    PUSHR_CODE =  9,
    JA_CODE    = 10,
    CALL_CODE  = 11,
    RET_CODE   = 12
};

int label_ctor (struct mtk* metkas);

int label_dtor (struct mtk* metkas);

int assembly (int* machine_code, struct mtk* metkas);

int filling_the_code (int* machine_code, struct mtk* metkas, int count);

int machine_code_dump (int count_itk, int* machine_code);

int main (void)
{
    int machine_code[MAX_ARR] = {};
    struct mtk metkas[MAX_SIZE] = {};

    for (int i = 0; i < MAX_SIZE; i++)
                metkas[i].addr = -1;

    int count_cmmnds = assembly (machine_code, metkas);
    machine_code_dump (count_cmmnds, machine_code);

    assembly (machine_code, metkas);
    machine_code_dump (count_cmmnds, machine_code);

    filling_the_code (machine_code, metkas, count_cmmnds);

    return 0;
}

/*int label_ctor (struct mtk* metka)
{
    assert(metka);
    metka->name_mtk = (char*) calloc ((size_t) MAX_SIZE, sizeof(metka->name_mtk[0]));
}*/

int assembly (int* machine_code, struct mtk* metkas)
{
    FILE* people_code = fopen("tests/math_exmpl.asm", "r");
    assert (people_code);

    int count_mtk = 0;
    int count_itr = 0;

    while (!feof(people_code))
    {
        char cmd[MAX_ARR] = {};

        if (fscanf(people_code, "%s", cmd) != 1)
            break;

        printf (">>>" BLUE_TEXT("[%02d = 0x%04x]") PURPLE_TEXT("cmd")" = '%s'\n", count_itr, (uint)count_itr, cmd);

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

        if (strcmp(cmd, "push") == 0)
        {
            machine_code[count_itr] = PUSH_CODE;
            count_itr++;

            int num = 0;

            fscanf(people_code, "%d", &num);

            machine_code[count_itr] = num;

            count_itr++;
        }
        else if (strcmp(cmd, "pop") == 0)
        {
            machine_code[count_itr] = POP_CODE;
            count_itr++;

            char num[MAX_SIZE] = {};

            fscanf(people_code, "%s", num);

            if (strcmp(num, "ax") == 0)
                machine_code[count_itr] = 1;
            if (strcmp(num, "bx") == 0)
                machine_code[count_itr] = 2;
            if (strcmp(num, "cx") == 0)
                machine_code[count_itr] = 3;

            count_itr++;
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
        else if (strcmp(cmd, "pushr") == 0)
        {
            machine_code[count_itr] = PUSHR_CODE;
            count_itr++;

            char num[MAX_SIZE] = {};

            fscanf(people_code, "%s", num);

            if (strcmp(num, "ax") == 0)
                machine_code[count_itr] = 1;
            if (strcmp(num, "bx") == 0)
                machine_code[count_itr] = 2;
            if (strcmp(num, "cx") == 0)
                machine_code[count_itr] = 3;

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
        else if (strcmp(cmd, "call") == 0)
        {
            machine_code[count_itr] = CALL_CODE;
            count_itr++;
        }
        else if (strcmp(cmd, "ret") == 0)
        {
            machine_code[count_itr] = RET_CODE;
            count_itr++;
        }
    }

    printf("\n-------------------------------------------------------------------------\n\n");

    fclose(people_code);

    return count_itr;
}

int filling_the_code (int* machine_code, struct mtk* /*metkas*/, int count)
{
    struct head header = { 0x584C4E44, VERSION, count };

    printf("%s(): count_cmmnds = %d\n", __FUNCTION__, count);

    FILE* machine = fopen("tests/machine_lg.bin", "wb");
    assert (machine);

    fwrite (&header     , sizeof (header)         , 1    , machine);
    fwrite (machine_code, sizeof (machine_code[0]), (size_t)count, machine);

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

    printf (PURPLE_TEXT("\n       %*s^ ip = %02d\n\n"), ip * 5, "", ip);

    /*printf (BLUE_TEXT("stack: "));
    for (int i = 0; i < count_itk; i++)
        printf("%s" BLUE_TEXT("[%d]") "%f", (i? ", " : ""), i, machine_code[i]);
     */
    printf (LIGHT_BLUE_TEXT("\n\n----------------------------------------------------------------------------------------------------------\n"));

    getchar ();

    return 0;
}
