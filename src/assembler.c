


#include <stdio.h>
#include <string.h>
#include <assert.h>

enum operartion_code
{
    PUSH = 1,
    POP = 2,
    ADD = 3,
    SUB = 4,
    MUL = 5,
    DIV = 6,
    OUT = 7,
    HLT = 8
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
            machine_code[count] = PUSH;
            count++;
        }
        else if (strcmp(cmd, "pop") == 0)
        {
            machine_code[count] = POP;
            count++;
        }
        else if (strcmp(cmd, "add") == 0)
        {
            machine_code[count] = ADD;
            count++;
        }
        else if (strcmp(cmd, "sub") == 0)
        {
            machine_code[count] = SUB;
            count++;
        }
        else if (strcmp(cmd, "mul") == 0)
        {
            machine_code[count] = MUL;
            count++;
        }
        else if (strcmp(cmd, "div") == 0)
        {
            machine_code[count] = DIV;
            count++;
        }
        else if (strcmp(cmd, "out") == 0)
        {
            machine_code[count] = OUT;
            count++;
        }
        else if (strcmp(cmd, "hlt") == 0)
        {
            machine_code[count] = HLT;
            count++;
        }

        if (strcmp(cmd, "push") == 0)
        {
            int c = 0;

            fscanf(people_code, "%d", &c);

            machine_code[count] = c;

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
