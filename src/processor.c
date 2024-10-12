#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"
#include "color_print.h"

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

struct SPU
{
    int* code;
    int ip;
    struct stack_str stack;
    int* registers;
};

int processor_dump (struct SPU* processor);

int processor_ctor (struct SPU* processor);

int processor_dtor (struct SPU* processor);

int main (void)
{
    struct SPU processor = {};
    processor_ctor(&processor);

    FILE* assm = fopen("text/machine_lg.txt", "r");
    assert(assm);

    for (int i = 0; i < 19; i++)
        fscanf(assm, "%d", &processor.code[i]);

    fclose(assm);

    /*for (int i = 0; i < 19; i++)
        printf("%d ", processor.code[i]);

    printf("\n");*/
    processor.ip = 0;

    int dnlx_is_alive = 1;
    while (dnlx_is_alive)
    {
        printf (">>> IP = %d, COMMAND: %d\n", processor.ip, processor.code[processor.ip]);
        processor_dump(&processor);

        switch (processor.code[processor.ip])
        {
            case PUSH:
            {
                printf (">>> processor.ip BEFORE +=: %d\n", processor.ip);

                stack_push(&processor.stack, processor.code[processor.ip + 1]);

                printf (">>> processor.ip BEFORE +=: %d\n", processor.ip);
                processor.ip += 2;
                printf (">>> processor.ip AFTER  +=: %d\n", processor.ip);

                printf (">>> END OF PUSH\n\n");

                break;

            /*case POP:
                stack_elem_t x = 0;
                stack_pop(&processor.stack, &x);*/
            }

            case ADD:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop(&processor.stack, &a);
                stack_pop(&processor.stack, &b);
                stack_push(&processor.stack, a + b);

                processor.ip += 1;
                break;
            }

            case SUB:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop(&processor.stack, &a);
                stack_pop(&processor.stack, &b);
                stack_push(&processor.stack, b - a);

                processor.ip += 1;
                break;
            }

            case MUL:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop(&processor.stack, &a);
                stack_pop(&processor.stack, &b);
                stack_push(&processor.stack, a * b);

                processor.ip += 1;
                break;
            }

            case DIV:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop(&processor.stack, &a);
                stack_pop(&processor.stack, &b);
                stack_push(&processor.stack, b / a);

                processor.ip += 1;
                break;
            }

            case OUT:
            {
                stack_elem_t x = 0;
                stack_pop(&processor.stack, &x);
                printf(RED_TEXT("OUTPUT: %g\n\n"), x);

                processor.ip += 1;
                break;
            }

            case HLT:
            {
                printf(RED_TEXT(">>> EXIT!!!\n\n"));
                dnlx_is_alive = 0;
                break;
            }

            default:
                assert(0);
        }
    }

    processor_dtor(&processor);

    return 0;
}

int processor_dump (struct SPU* processor)
{
    printf(LIGHT_BLUE_TEXT("----------------------------------------------------------------------------------------------------------\n\n"));

    int dnlx_limit = 19;

    printf(BLUE_TEXT("code:"));
    for (int i = 0; i < dnlx_limit; i++)
        printf(BLUE_TEXT("  %02d "), i);

    printf("\n     ");

    for (int i = 0; i < dnlx_limit; i++)
        printf(YELLOW_TEXT("  %02d "), processor->code[i]);

    printf(PURPLE_TEXT("\n       %*s^ ip = %02d\n\n"), processor->ip * 5, "", processor->ip);

    printf(BLUE_TEXT("stack: "));
    for (int i = 0; i < processor->stack.capacity; i++)
        printf("%s" BLUE_TEXT("[%d]") "%f", (i? ", " : ""), i, processor->stack.data[i]);

    printf(LIGHT_BLUE_TEXT("\n\n----------------------------------------------------------------------------------------------------------\n"));

    getchar();

    return 0;
}

int processor_ctor (struct SPU* processor)
{
    assert(processor);

    processor->ip = 0;

    processor->code = calloc (100, sizeof(int));

    processor->registers = calloc (10, sizeof(int));

    stack_ctor(&processor->stack, 2);


    if (processor->registers == NULL)
        assert(0);

    if (processor->code == NULL)
        assert(0);

    return 0;
}

int processor_dtor (struct SPU* processor)
{
    free(processor->code);
    free(processor->registers);

    stack_dtor (&processor->stack);

    return 0;
}
