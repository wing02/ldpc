#ifndef CODER_H_
#define CODER_H_

#include"ieee_ldpc_encode.h"
#include"ieee_ldpc_decode.h"
void bit_encode(char *input,int input_length,char * output,int output_length,int ldpc_k,int ldpc_n,enum rate_type rate);
void bit_decode(char *input,int input_length,char * output ,int output_length,int ldpc_k,int ldpc_n,enum rate_type rate);
#endif
