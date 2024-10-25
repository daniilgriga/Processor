#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "color_print.h"

#define MAX_SIZE 30
#define VERSION  2
#define MAX_ARR  250

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
    HLT_CODE   =  8,
    JNE_CODE   =  9,
    JA_CODE    = 10,
    CALL_CODE  = 11,
    RET_CODE   = 12,
    DRAW_CODE  = 13,
    JE_CODE    = 14,
    JB_CODE    = 15,
    SQRT_CODE  = 16,
    IN_CODE    = 17,
    OUT_CODE   = 18
};

int label_ctor (struct mtk* metkas);

int label_dtor (struct mtk* metkas);

int assembly (int* machine_code, struct mtk* metkas);

int filling_the_code (int* machine_code, struct mtk* metkas, int count);

int machine_code_dump (int count_itk, int* machine_code);

int compile_arg (char* cmd, int* machine_code, int* count);

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

            machine_code[count_itr] = metkas[num].addr;

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
    }

    printf("\n");

    fclose(people_code);

    return count_itr;
}

int filling_the_code (int* machine_code, struct mtk* /*metkas*/, int count)
{
    printf(BLUE_TEXT("%s():") PURPLE_TEXT("count_cmmnds")" = "YELLOW_TEXT("%d")"\n", __FUNCTION__, count);

    struct head header = { 0x584C4E44, VERSION, count };

    FILE* machine = fopen("tests/machine_lg.bin", "wb");
    assert (machine);

    fwrite (&header     , sizeof (header)         , 1    , machine);
    fwrite (machine_code, sizeof (machine_code[0]), (size_t)count, machine);

#if 0
    FILE* machine = fopen("tests/machine_lngg.txt", "w");
    assert (machine);

    for (int i = 0; i < count; i++)
        fprintf(machine, "%d  ", machine_code[i]);
#endif

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

        machine_code[(*count)++] = 0b00000111;
        machine_code[(*count)++] = num;
        machine_code[(*count)++] = str[0] - 'a' + 1;

        return 0;
    }

    if (have_bracket && !have_plus)
    {
        char str[MAX_SIZE] = {};
        int num = 0;

        int res_1 = sscanf (cmd, "[%1[a-d]x]", str);
        int res_2 = sscanf (cmd, "[%d]", &num);

        if (res_1 == 1)
        {
            machine_code[(*count)++] = 0b00000110;
            machine_code[(*count)++] = str[0] - 'a' + 1;
        }

        if (res_2 == 1)
        {
            machine_code[(*count)++] = 0b00000101;
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

        machine_code[(*count)++] = 0b00000011;
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
            machine_code[(*count)++] = 0b00000010;
            machine_code[(*count)++] = str[0] - 'a' + 1;
        }

        if (res_2 == 1)
        {
            machine_code[(*count)++] = 0b00000001;
            machine_code[(*count)++] = num;
        }

        return 0;
    }

    return 1;
}
