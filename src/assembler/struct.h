#ifndef STRUCT_ASM_H_
#define STRUCT_ASM_H_

#define MAX_SIZE 500
#define VERSION  2
#define MAX_ARR  250

const uint32_t SIGNATURE = 0x584C4E44;

struct header_t
{
    uint32_t signature;
    int      version;
    int      size;
};

struct metka_t
{
    char  name_mtk[MAX_SIZE];
    int   addr;
};

#endif //STRUCT_ASM_H_
