#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>

#include "../include/enum.h"
#include "stack.h"
#include "../include/color_print.h"
#include "struct.h"
#include "processor.h"

int main (const int argc, const char* argv[])
{
    const char* in_filename = (argc == 2)? argv[1] : "math_exmpl.bin";

    struct header_t header = {};
    struct SPU processor   = {};

    if (processor_ctor (&processor, &header, in_filename) == 1)
    {
        printf(RED_TEXT("CTOR ERROR!"));
        return 1;
    }

    machine_code_execution_func (&processor, &header);

    processor_dtor (&processor);

    return 0;
}

int machine_code_execution_func (struct SPU* processor, struct header_t* header)
{
    assert (processor);
    assert (header);

    int cycle = 1;
    while (cycle)
    {
        printf (">>> IP = %d, COMMAND: %d\n", processor->ip, processor->code[processor->ip]);

        processor_dump (processor, header);
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

                if (a != 0)
                    stack_push (&processor->stack, b / a);
                else
                    printf("you are trying to divide by zero...");

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
                {
                    processor->ip = processor->code[processor->ip + 1];
                    if (processor->ip >= header->size)
                    {
                        printf("ip < size!");
                        assert (0);
                    }

                }
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

	        case JMP_CODE:
            {
                processor->ip = processor->code[processor->ip + 1];

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

int load_code (struct SPU* processor, struct header_t* header, FILE* assm)
{
    assert (assm);
    assert (processor);
    assert (header);

    fread (processor->code, sizeof(int), (size_t) header->size, assm);

    //TODO - read bin. file; struct and machine_code

    fclose (assm);

    return 0;
}

int processor_dump (struct SPU* processor, struct header_t* header)
{
    printf (LIGHT_BLUE_TEXT("-----------------------------------------------------------------------------------------------------\n\n"));

    printf (BLUE_TEXT("code:"));
    for (int i = 0; i < header->size; i++)
        printf (BLUE_TEXT("  %03d "), i);

    printf("\n     ");

    for (int i = 0; i < header->size; i++)
        printf (GREEN_TEXT("  %03d "), processor->code[i]);

    printf (PURPLE_TEXT("\n       %*s^ ip = %02d\n\n"), processor->ip * 6, "", processor->ip);

    printf (BLUE_TEXT("stack: "));
    for (int i = 0; i < processor->stack.capacity; i++)
        printf("%s" BLUE_TEXT("[%d]") "%d", (i? ", " : ""), i, processor->stack.data[i]);

    printf(PURPLE_TEXT("\nregisters: "));
    for (int i = 0; i < REG_SIZE - 5; i++)
        printf("%s" PURPLE_TEXT("[%cx]")" = %d", (i? ", " : ""), 97 + i, processor->registers[i + 1]);

    printf("\n\nRAM:\n");
    for (int i = 0; i < RAM_SIZE/20; i++)
    {
        for (int j = 0; j < 20; j++) printf ("%02X ", (uint) processor->RAM [i*20 + j]);
        printf (" \t");
        for (int j = 0; j < 20; j++) printf ("%c ",          processor->RAM [i*20 + j]);
        printf ("\n");
    }
    printf ("\n");

    printf (LIGHT_BLUE_TEXT("\n\n-----------------------------------------------------------------------------------------------------\n"));

    printf(LIGHT_BLUE_TEXT("\nPress enter...\n"));
    getchar ();

    return 0;
}

int processor_ctor (struct SPU* processor, struct header_t* header, const char* filename)
{
    assert (processor);
    assert (header);

    processor->ip = 0;

    FILE* assm = fopen (filename, "rb");
    assert (assm);

    if (read_header (header, assm) == 1)
        return 1;

    processor->code = calloc ((size_t) header->size, sizeof(int));
    if (processor->code == NULL)
        return 1;

    load_code (processor, header, assm);

    if ( stack_ctor (&processor->stack, 2) != 0 )
        return 1;
    if ( stack_ctor (&processor->ret_addr_stack, 2) != 0 )
        return 1;

    for (int i = 1; i < REG_SIZE; i++)
        processor->registers[i] = 0;

    for (int i = 0; i < RAM_SIZE; i++)
        processor->RAM[i] = '.';

    processor_assert (processor);

    return 0;
}

int read_header (struct header_t* header, FILE* assm)
{
    assert (header);
    assert (assm);

    int size = count_symbols (assm);

    fread (header, sizeof(*header), 1, assm);

    // TODO count_symbols from onegin and compare with header->size
    printf ("\nfstat size = %ld\n", (size - sizeof(*header)) / sizeof(int) );
    printf ("\n\n"BLUE_TEXT("header->signature")" = 0x%d\n"BLUE_TEXT("header->version")" = %d\n"BLUE_TEXT("header->size")" = %d\n\n",
            (int) header->signature, header->version, header->size );

    if (header->size != (size - sizeof(*header)) / sizeof(int) )
    {
        printf("sizes dont equals, bro\n");
        return 1;
    }

    if ((int) header->signature != SIG)
    {
        printf("the processor is not for you, bro\n");
        return 1;
    }

    if (header->version != VER)
    {
        printf("the processor does not support this version, bro\n");
        return 1;
    }

    return 0;
}

int count_symbols (FILE* file)
{
    if (fseek(file, 0, SEEK_END) != 0)
        return -1;

    struct stat statbuf;

    int count = fstat(fileno (file), &statbuf);
    if (count == -1)
        return -1;

    if (fseek(file, 0, SEEK_SET) != 0)
        return -1;

    return statbuf.st_size;
}

int verifier (struct SPU* processor)
{
    assert (processor);

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
        case PROCESSOR_IS_NULL:       printf(RED_TEXT(" - PROCESSOR_IS_NULL"));       break;
        case PROCESSOR_CODE_IS_NULL:  printf(RED_TEXT(" - PROCESSOR_CODE_IS_NULL"));  break;
        case PROCESSOR_BAD_IP:        printf(RED_TEXT(" - PROCESSOR_BAD_IP"));        break;
        case PROCESSOR_STACK_IS_NULL: printf(RED_TEXT(" - PROCESSOR_STACK_IS_NULL")); break;
        default:
            printf(RED_TEXT(" - UNKNOWN ERROR IN PROCESSOR"));
    }

    printf("\n");

    return 0;
}

int processor_assert (struct SPU* processor)
{
    assert (processor);

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
    assert (processor);

    free (processor->code);

    stack_dtor (&processor->stack);
    stack_dtor (&processor->ret_addr_stack);

    return 0;
}

int* get_arg (struct SPU* processor)
{
    assert (processor);

    int  op_code    = processor->code[processor->ip++]; (void)op_code;
    int  arg_type   = processor->code[processor->ip++];
    int  arg_value  = 0;
    int* value_addr = NULL;

    if (arg_type & IMMED_ARG) { value_addr = &processor->code[processor->ip];
                                arg_value  =  processor->code[processor->ip++]; }

    if (arg_type & REG_ARG)   { value_addr = &processor->registers[processor->code[processor->ip]];
                                arg_value +=  processor->registers[processor->code[processor->ip++]]; }

    if (arg_type & MEM_ARG)   { value_addr = &processor->RAM[arg_value]; }

    return value_addr;
}

