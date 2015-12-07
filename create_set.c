#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include"matrix.h"
#include"ieee_ldpc_encode.h"

const int z=24;
const int ldpc_n = z*24;
const int ldpc_k = ldpc_n/2;
const int ldpc_m = ldpc_n-ldpc_k;
const enum rate_type rate = rate_1_2;

int main(){

    FILE *fpx,*fpy;
    int set_size;
    int i,j; 
    matrix * matrix_h,*matrix_n,*matrix_k;

    fpx=fopen("set/train_x","w+");
    fpy=fopen("set/train_y","w+");
    matrix_h = ieee_ldpc_get_h(ldpc_n,rate_1_2);
    set_size=50000;
    srand( (unsigned int)time(0) );
    char *no_code=(char*)malloc(sizeof(char)*ldpc_k);
    for(i=0;i<set_size;i++){
        for(j=0;j<ldpc_k;j++){
            no_code[j]=rand()%2;
            fprintf(fpy,"%d ",no_code[j]);
        }
        matrix_k = matrix_from_array(no_code,1,ldpc_k);
        matrix_n = ieee_ldpc_encode(matrix_k,matrix_h);
        for(j=0;j<ldpc_n;j++){
            fprintf(fpx,"%d ",matrix_get(matrix_n,0,j));
        }

        fprintf(fpx,"\n");
        fprintf(fpy,"\n");
        matrix_free(matrix_k);
        matrix_free(matrix_n);

    }
    matrix_free(matrix_h);

    fclose(fpx);
    fclose(fpy);

}
