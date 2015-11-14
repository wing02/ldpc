#include"matrix.h"
#include"ieee_ldpc_encode.h"
#include"ieee_ldpc_decode.h"
#include<stdio.h>
#include<time.h>
int main(){
    FILE * fp=fopen("result.txt","a+");
    int ldpc_n,ldpc_k,ldpc_m,i;
    clock_t start,finish;
    int ERRNUM,iter_time;
    matrix * matrix_h,*matrix_n,*matrix_k;
    matrix * matrix_n1,*matrix_n2;
    int z=56;

    ldpc_n = z*24;
    fprintf(fp,"%d\t",ldpc_n);
    printf("ldpc_n=%d\n",ldpc_n/24);
    ldpc_k = ldpc_n/6*5;
    ldpc_m = ldpc_n-ldpc_k;
    ERRNUM=ldpc_n/10;
    //ERRNUM=0;
    matrix_h = ieee_ldpc_get_h(ldpc_n,rate_5_6);
    matrix_k = matrix_alloc(1,ldpc_k);
    for(i=0;i<ldpc_k;i++){
        matrix_set(matrix_k,0,i,i%2);
    }
    //*********************encode
    start=clock();
    matrix_n = ieee_ldpc_encode(matrix_k,matrix_h);
    finish=clock();
    //printf("encode time= %f seconds\n",(float)(finish-start)/CLOCKS_PER_SEC );
    fprintf(fp,"%f\t",(float)(finish-start)/CLOCKS_PER_SEC );
    //printf("matrix_n(src)=\n");
    //matrix_printf(matrix_n);
    //printf("matrix_k(src)=\n");
    //matrix_printf(matrix_k);
    srand((unsigned)time(NULL));
    i=ERRNUM;
    while(i--){
        int num=rand()%ldpc_n;
        while(matrix_get(matrix_n,0,num)==ERR)
            num=rand()%ldpc_n;
        matrix_set(matrix_n,0,num,ERR);
    }
    printf("matrix_n=\n");
    matrix_printf(matrix_n);

    //*********************************decode
    matrix_n1=matrix_alloc(1,ldpc_n);
    matrix_n2=matrix_alloc(1,ldpc_n);
    matrix_copy(matrix_n,matrix_n1);
    matrix_copy(matrix_n,matrix_n2);

    start=clock();
    iter_time=cl_decode(matrix_n1,matrix_h);
    finish=clock();
    printf("iter_time=%d\n",iter_time);
    printf("cl_decode time= %f seconds\n",(float)(finish-start)/CLOCKS_PER_SEC );
    fprintf(fp,"%d\t%f\t",iter_time,(float)(finish-start)/CLOCKS_PER_SEC );

    start=clock();
    iter_time=decode(matrix_n2,matrix_h);
    finish=clock();
    printf("iter_time=%d\n",iter_time);
    printf("decode time= %f seconds\n",(float)(finish-start)/CLOCKS_PER_SEC );
    fprintf(fp,"%d\t%f\n",iter_time,(float)(finish-start)/CLOCKS_PER_SEC );

    printf("Error Num=%d\n",ERRNUM);
    printf("matrix_gpu(result)=\n");
    matrix_printf(matrix_n1);
    printf("matrix_cpu(result)=\n");
    matrix_printf(matrix_n2);
    printf("****************************************************************************************************************************\n");
    fclose(fp);
    return 0;
}
