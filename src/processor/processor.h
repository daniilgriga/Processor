#ifndef PROCESSOR_H_
#define PROCESSOR_H

const int SIG = 1481395780;
const int VER = 2;

int  processor_dump (struct SPU* processor, struct header_t* header);

int  processor_ctor (struct SPU* processor, struct header_t* header, const char* filename);

int  processor_dtor (struct SPU* processor);

int  verifier (struct SPU* processor);

int  proc_error_code_output (int processor_err);

int  processor_assert (struct SPU* processor);

int  load_code (struct SPU* processor, struct header_t* header, FILE* assm);

int* get_arg (struct SPU* processor);

int  machine_code_execution_func (struct SPU* processor, struct header_t* header);

int read_header (struct header_t* header, FILE* assm);

int count_symbols (FILE* file);

#endif
