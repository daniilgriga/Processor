#ifndef STRUCT_PROC_H_
#define STRUCT_PROC_H_

#define REG_SIZE 10
#define RAM_SIZE 300

struct SPU
{
    int*              code;
    int               ip;
    struct stack_str  stack;
    int               registers[REG_SIZE];
    int               RAM[RAM_SIZE];
    struct stack_str  ret_addr_stack;
};

struct header_t
{
    uint32_t  signature;
    int       version;
    int       size;
};

#endif //STRUCT_PROC_H_
