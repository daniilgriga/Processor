#ifndef ENUM_H_
#define ENUM_H_

#define DEF_CMD_(cmd, num, ...) cmd##_CODE = num,

enum OperationCode
{
    #include "commands.h"
    LAST_CODE = 50
};

#undef DEF_CMD_

enum errors_in_proccesor
{
    PROCESSOR_OK            = 0,
    PROCESSOR_IS_NULL       = 1,
    PROCESSOR_CODE_IS_NULL  = 2,
    PROCESSOR_BAD_IP        = 4,
    PROCESSOR_STACK_IS_NULL = 8
};

enum bites
{
    IMMED_ARG = 0b00000001,
    REG_ARG   = 0b00000010,
    MEM_ARG   = 0b00000100
};

#endif // ENUM_H_
/*
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
*/
