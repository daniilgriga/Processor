#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include "stack.h"
#include "color_print.h"

#define REG_SIZE 10
#define RAM_SIZE 250

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

enum errors_in_proccesor
{
    PROCESSOR_OK            = 0,
    PROCESSOR_IS_NULL       = 1,
    PROCESSOR_CODE_IS_NULL  = 2,
    PROCESSOR_BAD_IP        = 4,
    PROCESSOR_STACK_IS_NULL = 8
};

struct SPU
{
    int*                code;
    int                 ip;
    struct stack_str    stack;
    int                 registers[REG_SIZE];
    int                 RAM[RAM_SIZE];
    struct stack_str    ret_addr_stack;
};

struct head
{
    uint32_t sig;
    int      ver;
    int      size;
};

int processor_dump (struct SPU* processor, struct head* header);

int processor_ctor (struct SPU* processor, struct head* header);

int processor_dtor (struct SPU* processor);

int verifier (struct SPU* processor);

int proc_error_code_output (int processor_err);

int processor_assert (struct SPU* processor);

int filling_the_code (struct SPU* processor, struct head* header);

int* get_arg (struct SPU* processor);

int machine_code_execution_func (struct SPU* processor, struct head* header, int DerDebug);

int main (int argc, const char* argv[])
{
    int DerDebug = (argc >= 2 && strcmp (argv[1], "-d") == 0); //TODO - global var

    struct head header   = {};
    struct SPU processor = {};

    processor_ctor (&processor, &header);

    machine_code_execution_func (&processor, &header, DerDebug);

    processor_dtor (&processor);

    return 0;
}

int machine_code_execution_func (struct SPU* processor,  struct head* header, int DerDebug)
{
    assert (processor);
    assert (header);

    int cycle = 1;
    while (cycle)
    {
        printf (">>> IP = %d, COMMAND: %d\n", processor->ip, processor->code[processor->ip]);
        if (DerDebug) processor_dump(processor, header);

        switch (processor->code[processor->ip])
        {
            case PUSH_CODE:
            {
                stack_push (&processor->stack, *get_arg(processor));

                break;
            }

            case POP_CODE:
            {
                int* ptr = get_arg(processor);
                stack_pop (&processor->stack, ptr);

                break;
            }

            case ADD_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop  (&processor->stack, &a);
                stack_pop  (&processor->stack, &b);
                stack_push (&processor->stack, a + b);

                processor->ip += 1;

                break;
            }

            case SUB_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop  (&processor->stack, &a);
                stack_pop  (&processor->stack, &b);
                stack_push (&processor->stack, b - a);

                processor->ip += 1;

                break;
            }

            case MUL_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop  (&processor->stack, &a);
                stack_pop  (&processor->stack, &b);
                stack_push (&processor->stack, a * b);

                processor->ip += 1;

                break;
            }

            case DIV_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop  (&processor->stack, &a);
                stack_pop  (&processor->stack, &b);
                stack_push (&processor->stack, b / a);

                processor->ip += 1;

                break;
            }

            case OUT_CODE:
            {
                stack_elem_t x = 0;
                stack_pop (&processor->stack, &x);

                printf(RED_TEXT("\n\n>>> >>> OUTPUT: %d\n\n"), x);

                processor->ip += 1;

                break;
            }

            case HLT_CODE:
            {
                printf(RED_TEXT(">>> EXIT!!!\n\n"));

                cycle = 0;

                break;
            }

            case CALL_CODE:
            {
                stack_push (&processor->ret_addr_stack, processor->ip + 2);

                processor->ip = processor->code[processor->ip + 1];

                break;
            }

            case RET_CODE:
            {
                stack_elem_t addr = 0;
                stack_pop (&processor->ret_addr_stack, &addr);

                processor->ip = addr;

                break;
            }

            case JA_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop (&processor->stack, &a);
                stack_pop (&processor->stack, &b);

                if (a > b)
                    processor->ip = processor->code[processor->ip + 1];
                else
                    processor->ip += 2;

                break;
            }

            case JB_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop (&processor->stack, &a);
                stack_pop (&processor->stack, &b);

                if (a < b)
                    processor->ip = processor->code[processor->ip + 1];
                else
                    processor->ip += 2;

                break;
            }

            case JE_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop (&processor->stack, &a);
                stack_pop (&processor->stack, &b);

                if (a == b)
                    processor->ip = processor->code[processor->ip + 1];
                else
                    processor->ip += 2;

                break;
            }

            case JNE_CODE:
            {
                stack_elem_t a = 0;
                stack_elem_t b = 0;
                stack_pop (&processor->stack, &a);
                stack_pop (&processor->stack, &b);

                if (a != b)
                    processor->ip = processor->code[processor->ip + 1];
                else
                    processor->ip += 2;

                break;
            }

            case DRAW_CODE:
            {
                for (int i = 0; i < RAM_SIZE; i++)
                {
                    if (i % 20 == 0) printf ("\n");
                    printf ("%c", processor->RAM[i]);
                }
                printf ("\n");

                processor->ip++;

                break;
            }

            case SQRT_CODE:
            {
                stack_elem_t a = 0;
                stack_pop  (&processor->stack, &a);
                stack_elem_t num = (int) sqrt (a);
                stack_push (&processor->stack, num);

                processor->ip += 1;

                break;
            }

            case IN_CODE:
            {
                int num = 0;
                scanf("%d", &num);

                stack_push (&processor->stack, num);

                processor->ip += 1;

                break;
            }

            default:
                printf (PURPLE_TEXT("Unknown command: '%d'\n"), processor->code[processor->ip]);
                assert(0);
        }
    }

    return 0;
}

int filling_the_code (struct SPU* processor, struct head* header)
{
    FILE* assm = fopen ("tests/machine_lg.bin", "rb");

    assert (assm);
    fread (processor->code, sizeof(*header), 1, assm);
                                                            //TODO - read bin. file; struct and machine_code
    header->sig  = (uint32_t) processor->code[0];
    header->ver  = processor->code[1];
    header->size = processor->code[2];
    printf ("header->sig = %d, header->ver = %d, header->size = %d\n", processor->code[0], processor->code[1], processor->code[2] );
    fread (processor->code, sizeof(int), (size_t) header->size, assm);

#if 0
    FILE* assm = fopen("tests/machine_lngg.txt", "r");
    assert(assm);

    header->size = 24;

    for (int i = 0; i < header->size; i++)
        fscanf(assm, "%d", &processor->code[i]);
#endif

    fclose (assm);

    return 0;
}

int processor_dump (struct SPU* processor, struct head* header)
{
    printf (LIGHT_BLUE_TEXT("-----------------------------------------------------------------------------------------------------\n\n"));

    printf (BLUE_TEXT("code:"));
    for (int i = 0; i < header->size; i++)
        printf (BLUE_TEXT("  %02d "), i);

    printf("\n     ");

    for (int i = 0; i < header->size; i++)
        printf (GREEN_TEXT("  %02d "), processor->code[i]);

    printf (PURPLE_TEXT("\n       %*s^ ip = %02d\n\n"), processor->ip * 5, "", processor->ip);

    printf (BLUE_TEXT("stack: "));
    for (int i = 0; i < processor->stack.capacity; i++)
        printf("%s" BLUE_TEXT("[%d]") "%d", (i? ", " : ""), i, processor->stack.data[i]);

    printf("\nregisters: "BLUE_TEXT("[ax]")" = %d  "BLUE_TEXT("[bx]")" = %d  "BLUE_TEXT("[cx]")" = %d  "BLUE_TEXT("[dx]")" = %d",
            processor->registers[1], processor->registers[2], processor->registers[3], processor->registers[4]);

    /*printf("\n\nRAM:\n");
    for (int i = 0; i < RAM_SIZE/20; i++)
    {
        for (int j = 0; j < 20; j++) printf ("%02X ", (uint) processor->RAM [i*20 + j]);
        printf (" \t");
        for (int j = 0; j < 20; j++) printf ("%c ",          processor->RAM [i*20 + j]);
        printf ("\n");
    }
    printf ("\n");*/


    printf (LIGHT_BLUE_TEXT("\n\n-----------------------------------------------------------------------------------------------------\n"));

    getchar ();

    return 0;
}

int processor_ctor (struct SPU* processor, struct head* header)
{
    assert (processor);

    processor->ip = 0;  //TODO - read_header_func

    processor->code = calloc ((size_t) RAM_SIZE, sizeof(int)); //TODO - header->size

    filling_the_code (processor, header); //TODO - load_code

    stack_ctor (&processor->stack, 2);
    stack_ctor (&processor->ret_addr_stack, 2);

    for (int i = 1; i < REG_SIZE; i++)
        processor->registers[i] = 0;

    for (int i = 0; i < RAM_SIZE; i++)
        processor->RAM[i] = 0;

    processor_assert (processor);

    return 0;
}

int verifier (struct SPU* processor)
{
    int processor_err = 0;

    if (processor == NULL)
        processor_err |= PROCESSOR_IS_NULL;

    if (processor->code == NULL)
        processor_err |= PROCESSOR_CODE_IS_NULL;

    if (processor->ip < 0)
        processor_err |= PROCESSOR_BAD_IP;

    return processor_err;
}

int proc_error_code_output (int processor_err)
{
    printf (RED_TEXT("ERROR IN PROCESSOR: "));

    convert_to_binary (processor_err);

    switch (processor_err)
    {
        case PROCESSOR_IS_NULL:
            printf(RED_TEXT(" - PROCESSOR_IS_NULL"));
            break;

        case PROCESSOR_CODE_IS_NULL:
            printf(RED_TEXT(" - PROCESSOR_CODE_IS_NULL"));
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

    if (processor->code == NULL)
        fprintf(stderr, YELLOW_TEXT("processor = NULL\n"));

    if (processor->ip < 0)
        fprintf(stderr, YELLOW_TEXT("processor->ip = %d\n"), processor->ip);

    stack_assert (&processor->stack, __FILE__, __LINE__, __FUNCTION__);

    assert(0);

    return 0;
}

int processor_dtor (struct SPU* processor)
{
    free (processor->code);

    stack_dtor (&processor->stack);
    stack_dtor (&processor->ret_addr_stack);

    return 0;
}

int* get_arg (struct SPU* processor)
{
    int  op_code    = processor->code[processor->ip++]; (void)op_code;
    int  arg_type   = processor->code[processor->ip++];
    int  arg_value  = 0;
    int* value_addr = NULL;

    if (arg_type & 1) { value_addr = &processor->code[processor->ip];
                        arg_value  =  processor->code[processor->ip++]; }

    if (arg_type & 2) { value_addr = &processor->registers[processor->code[processor->ip]];
                        arg_value +=  processor->registers[processor->code[processor->ip++]]; }

    if (arg_type & 4) { value_addr = &processor->RAM[arg_value]; }

    return value_addr;
}

