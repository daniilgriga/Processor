#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"
#include "color_print.h"

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


enum errors_in_proc
{
    PROCESSOR_OK                    = 0,
    PROCESSOR_IS_NULL               = 1,
    PROCESSOR_CODE_IS_NULL          = 2,
    PROCESSOR_REGISTERS_IS_NULL     = 4,
    PROCESSOR_BAD_IP                = 8,
    PROCESSOR_STACK_IS_NULL         = 16
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

int verifier (struct SPU* processor);

int proc_error_code_output (int processor_err);

int processor_assert (struct SPU* processor);

int filling_the_code (struct SPU* processor);

int main (void)
{
    struct SPU processor = {};
    processor_ctor(&processor);

    processor.ip = 0;

    double summ = 0;

    int cycle = 1;
    while (cycle)
    {
        printf (">>> IP = %d, COMMAND: %d\n", processor.ip, processor.code[processor.ip]);
        processor_dump(&processor);

        switch (processor.code[processor.ip])
        {
            case PUSH_CODE:
            {
                stack_push(&processor.stack, processor.code[processor.ip + 1]);

                processor.ip += 2;
                break;
            }

            case PUSHR_CODE:
            {
                stack_push(&processor.stack, processor.registers[processor.code[processor.ip + 1]]);

                processor.ip += 2;
                break;
            }

            case POP_CODE:
            {
                stack_elem_t a = 0;
                stack_pop(&processor.stack, &a);

                processor.registers[0] = (int) a;

                processor.ip += 2;
                break;
            }

            case ADD_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop(&processor.stack, &a);
                stack_pop(&processor.stack, &b);
                stack_push(&processor.stack, a + b);

                processor.ip += 1;
                break;
            }

            case SUB_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop(&processor.stack, &a);
                stack_pop(&processor.stack, &b);
                stack_push(&processor.stack, b - a);

                processor.ip += 1;
                break;
            }

            case MUL_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop(&processor.stack, &a);
                stack_pop(&processor.stack, &b);
                stack_push(&processor.stack, a * b);

                processor.ip += 1;
                break;
            }

            case DIV_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop(&processor.stack, &a);
                stack_pop(&processor.stack, &b);
                stack_push(&processor.stack, b / a);

                processor.ip += 1;
                break;
            }

            case OUT_CODE:
            {
                stack_elem_t x = 0;
                stack_pop(&processor.stack, &x);

                summ += x;

                printf(RED_TEXT("OUTPUT: %g\n\n"), x);

                processor.ip += 1;
                break;
            }

            case HLT_CODE:
            {
                printf(RED_TEXT(">>> EXIT!!!\n\n"));
                cycle = 0;
                break;
            }

            case JA_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                a = processor.stack.data[1];
                b = processor.stack.data[0];

                stack_elem_t x = 0;
                stack_pop(&processor.stack, &x);
                stack_pop(&processor.stack, &x);

                if (a >= b)
                {
                    processor.ip = processor.code[processor.ip + 1];
                }
                else
                {
                    cycle = 0;
                }

                break;
            }

            default:
                assert(0);
        }
    }

    printf("\n\n%g\n\n", summ);

    processor_dtor(&processor);

    return 0;
}

int filling_the_code (struct SPU* processor)
{
    FILE* assm = fopen("text/machine_lg.txt", "r");
    assert(assm);

    for (int i = 0; i < 24; i++)
        fscanf(assm, "%d", &processor->code[i]);

    fclose(assm);

    return 0;
}

int processor_dump (struct SPU* processor)
{
    printf(LIGHT_BLUE_TEXT("----------------------------------------------------------------------------------------------------------\n\n"));

    int size = 24;

    printf(BLUE_TEXT("code:"));
    for (int i = 0; i < size; i++)
        printf(BLUE_TEXT("  %02d "), i);

    printf("\n     ");

    for (int i = 0; i < size; i++)
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

    filling_the_code (processor);

    processor->registers = calloc (10, sizeof(int));

    stack_ctor(&processor->stack, 2);

    processor_assert(processor);

    return 0;
}

int verifier (struct SPU* processor)
{
    int processor_err = 0;

    if (processor == NULL)
        processor_err |= PROCESSOR_IS_NULL;

    if (processor->registers == NULL)
        processor_err |= PROCESSOR_REGISTERS_IS_NULL;

    if (processor->code == NULL)
        processor_err |= PROCESSOR_CODE_IS_NULL;

    if (processor->ip < 0)
        processor_err |= PROCESSOR_BAD_IP;


    return processor_err;
}

int proc_error_code_output (int processor_err)
{
    printf(RED_TEXT("ERROR IN PROCESSOR: "));

    convert_to_binary(processor_err);

    switch (processor_err)
    {
        case PROCESSOR_IS_NULL:
            printf(RED_TEXT(" - PROCESSOR_IS_NULL"));
            break;

        case PROCESSOR_CODE_IS_NULL:
            printf(RED_TEXT(" - PROCESSOR_CODE_IS_NULL"));
            break;

        case PROCESSOR_REGISTERS_IS_NULL:
            printf(RED_TEXT(" - PROCESSOR_REGISTERS_IS_NULL"));
            break;

        case PROCESSOR_BAD_IP:
            printf(RED_TEXT(" - PROCESSOR_BAD_IP"));
            break;

        case PROCESSOR_STACK_IS_NULL:
            printf(RED_TEXT(" - PROCESSOR_STACK_IS_NULL"));
            break;

        default:
            printf(RED_TEXT(" - UNKNOWN ERROR IN PROCESSOR"));
    }

    printf("\n");

    return 0;
}

int processor_assert (struct SPU* processor)
{
    if (!verifier(processor))
        return 0;

    fprintf(stderr, RED_TEXT("PROCESSOR IS NOT OKAY:\n"));

    error_code_output(verifier(processor));

    if (processor == NULL)
        fprintf(stderr, YELLOW_TEXT("processor = NULL\n"));

    if (processor->registers == NULL)
        fprintf(stderr, YELLOW_TEXT("stack->data = NULL\n"));

    if (processor->code == NULL)
        fprintf(stderr, YELLOW_TEXT("processor = NULL\n"));

    if (processor->ip < 0)
        fprintf(stderr, YELLOW_TEXT("processor->ip = %d\n"), processor->ip);

    stack_assert(&processor->stack, __FILE__, __LINE__, __FUNCTION__);

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
