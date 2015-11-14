#include<stdio.h>
#include<time.h>
#include"coder.h"
const int z = 56;
const int ldpc_n = z*24;
const int ldpc_k = ldpc_n/6*5;
const int ldpc_m = ldpc_n-ldpc_k;
const enum rate_type rate = rate_5_6;


int main(){
    long int nocode_length,encode_length,decode_length;
    //init the input array;
    nocode_length=1000;
    char *no_code=(char*)malloc(sizeof(char)*nocode_length);
    int i;
    for(i=0;i<nocode_length;i++){
        no_code[i]='a'+i%26;
    }
    //init the encode and decode array;
    encode_length=(nocode_length+ldpc_k/8-1)/(ldpc_k/8)*(ldpc_n/8);
    char * en_code=(char*)malloc(sizeof(char)*encode_length);
    decode_length=nocode_length;
    char * de_code=(char*)malloc(sizeof(char)*nocode_length);


    for(i=0;i<nocode_length;i++){
        printf("%c ",no_code[i]);
    }
    printf("\n\n");

    bit_encode(no_code,nocode_length,en_code,encode_length,ldpc_k,ldpc_n,rate_5_6);
    for(i=0;i<encode_length;i++){
        printf("%c ",en_code[i]);
    }
    printf("\n\n");

    bit_decode(en_code,encode_length,de_code,decode_length,ldpc_k,ldpc_n,rate_5_6);
    for(i=0;i<decode_length;i++){
        printf("%c ",de_code[i]);
    }
    printf("\n\n");
}
