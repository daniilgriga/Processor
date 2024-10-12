


#include <stdio.h>
#include <string.h>
#include <assert.h>

enum operartion_code
{
    PUSH_CODE = 1,
    POP_CODE = 2,
    ADD_CODE = 3,
    SUB_CODE = 4,
    MUL_CODE = 5,
    DIV_CODE = 6,
    OUT_CODE = 7,
    HLT_CODE = 8,
    PUSHR_CODE = 9,
    JA_CODE = 10
};

int main (void)
{
    FILE* people_code = fopen("text/math_exmpl.txt", "r");
    assert (people_code);

    char machine_code[100] = {};
    int count = 0;
    while (!feof(people_code))
    {
        char cmd[10] = {};

        if (fscanf(people_code, "%s", cmd) != 1)
            break;

        if (strcmp(cmd, "push") == 0)
        {
            machine_code[count] = PUSH_CODE;
            count++;
        }
        else if (strcmp(cmd, "pop") == 0)
        {
            machine_code[count] = POP_CODE;
            count++;
        }
        else if (strcmp(cmd, "add") == 0)
        {
            machine_code[count] = ADD_CODE;
            count++;
        }
        else if (strcmp(cmd, "sub") == 0)
        {
            machine_code[count] = SUB_CODE;
            count++;
        }
        else if (strcmp(cmd, "mul") == 0)
        {
            machine_code[count] = MUL_CODE;
            count++;
        }
        else if (strcmp(cmd, "div") == 0)
        {
            machine_code[count] = DIV_CODE;
            count++;
        }
        else if (strcmp(cmd, "out") == 0)
        {
            machine_code[count] = OUT_CODE;
            count++;
        }
        else if (strcmp(cmd, "hlt") == 0)
        {
            machine_code[count] = HLT_CODE;
            count++;
        }
        else if (strcmp(cmd, "pushr") == 0)
        {
            machine_code[count] = PUSHR_CODE;
            count++;
        }
        else if (strcmp(cmd, "ja") == 0)
        {
            machine_code[count] = JA_CODE;
            count++;
        }

        if (strcmp(cmd, "push") == 0 || strcmp(cmd, "ja") == 0)
        {
            int c = 0;

            fscanf(people_code, "%d", &c);

            machine_code[count] = c;

            count++;
        }

        if (strcmp(cmd, "pushr") == 0 || strcmp(cmd, "pop") == 0)
        {
            char c[2] = {};

            fscanf(people_code, "%s", c);

            if (c == "ax")
                machine_code[count] = 0;
            if (c == "bx")
                machine_code[count] = 1;
            if (c == "cx")
                machine_code[count] = 2;

            count++;
        }
    }

    printf("%s(): count = %d\n", __FUNCTION__, count);
    FILE* machine = fopen("text/machine_lg.txt", "w");
    assert (people_code);

    for (int i = 0; i < count; i++)
        fprintf(machine, "%d  ", machine_code[i]);

    fclose(people_code);
    fclose(machine);

    return 0;
}
