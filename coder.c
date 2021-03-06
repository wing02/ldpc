#include"matrix.h"
#include"char2bool.h"
#include"coder.h"

void bit_encode(char *input,int input_length,char * output,int output_length,int ldpc_k,int ldpc_n,enum rate_type rate){
    
    matrix * matrix_h,*matrix_n,*matrix_k;
    matrix_h = ieee_ldpc_get_h(ldpc_n,rate_5_6);
    int i;
    for(i=0;i<input_length;i+=ldpc_k/8){
        int data_length;
        if(i+ldpc_k/8>input_length){
            data_length=input_length-i;
        }else{
            data_length=ldpc_k/8;
        }
        char c[ldpc_k];
        bool2char(&input[i],c,ldpc_k/8,data_length);
        matrix_k=matrix_from_array(c,1,ldpc_k);
        matrix_n = ieee_ldpc_encode(matrix_k,matrix_h);
        char n_array[ldpc_n];
        int j;
        for(j=0;j<ldpc_n;j++){
            n_array[j]=matrix_get(matrix_n,0,j);
        }
        char2bool(n_array,&output[i/(ldpc_k/8)*(ldpc_n/8)],ldpc_n/8);
        matrix_free(matrix_k);
        matrix_free(matrix_n);
    }
    matrix_free(matrix_h);
}

void bit_decode(char *input,int input_length,char * output ,int output_length,int ldpc_k,int ldpc_n,enum rate_type rate){

    matrix * matrix_h,*matrix_n,*matrix_k;
    matrix_h = ieee_ldpc_get_h(ldpc_n,rate_5_6);
    int i;
    for(i=0;i<input_length;i+=ldpc_n/8){
        char c[ldpc_n];
        bool2char(&input[i],c,ldpc_n/8,ldpc_n/8);
        matrix_n=matrix_from_array(c,1,ldpc_n);
        cl_decode(matrix_n,matrix_h);
        char k_array[ldpc_k];
        int j;
        for(j=0;j<ldpc_k;j++){
            k_array[j]=matrix_get(matrix_n,0,j);
        }
        char2bool(k_array,&output[i/(ldpc_n/8)*(ldpc_k/8)],ldpc_k/8);
        matrix_free(matrix_n);
    }
    matrix_free(matrix_h);
}
