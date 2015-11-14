#include"ieee_ldpc_encode.h"
#include<stdio.h>
const char h_seed_1_2[]={
    -1,94,73,-1,-1,-1,-1,-1,55,83,-1,-1,7,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,27,-1,-1,-1,22,79,9,-1,-1,-1,12,-1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,24,22,81,-1,33,-1,-1,-1,0,-1,-1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,61,-1,47,-1,-1,-1,-1,-1,65,25,-1,-1,-1,-1,-1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,39,-1,-1,-1,84,-1,-1,41,72,-1,-1,-1,-1,-1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,46,40,-1,82,-1,-1,-1,79,0,-1,-1,-1,-1,0,0,-1,-1,-1,-1,-1,-1,-1,95,53,-1,-1,-1,-1,-1,14,18,-1,-1,-1,-1,-1,-1,-1,0,0,-1,-1,-1,-1,-1,11,73,-1,-1,-1,2,-1,-1,47,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,-1,-1,-1,12,-1,-1,-1,83,24,-1,43,-1,-1,-1,51,-1,-1,-1,-1,-1,-1,-1,-1,0,0,-1,-1,-1,-1,-1,-1,-1,94,-1,59,-1,-1,70,72,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,-1,-1,-1,7,65,-1,-1,-1,-1,39,49,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,43,-1,-1,-1,-1,66,-1,41,-1,-1,-1,26,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0
};
const char h_seed_2_3_a[]={
    3,0,-1,-1,2,0,-1,3,7,-1,1,1,-1,-1,-1,-1,1,0,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,36,-1,-1,34,10,-1,-1,18,2,-1,3,0,-1,0,0,-1,-1,-1,-1,-1,-1,-1,12,2,-1,15,-1,40,-1,3,-1,15,-1,2,13,-1,-1,-1,0,0,-1,-1,-1,-1,-1,-1,19,24,-1,3,0,-1,6,-1,17,-1,-1,-1,8,39,-1,-1,-1,0,0,-1,-1,-1,20,-1,6,-1,-1,10,29,-1,-1,28,-1,14,-1,38,-1,-1,0,-1,-1,-1,0,0,-1,-1,-1,-1,10,-1,28,20,-1,-1,8,-1,36,-1,9,-1,21,45,-1,-1,-1,-1,-1,0,0,-1,35,25,-1,37,-1,21,-1,-1,5,-1,-1,0,-1,4,20,-1,-1,-1,-1,-1,-1,-1,0,0,-1,6,6,-1,-1,-1,4,-1,14,30,-1,3,36,-1,14,-1,1,-1,-1,-1,-1,-1,-1,0
};
const char h_seed_2_3_b[]={
    2,-1,19,-1,47,-1,48,-1,36,-1,82,-1,47,-1,15,-1,95,0,-1,-1,-1,-1,-1,-1,
    -1,69,-1,88,-1,33,-1,3,-1,16,-1,37,-1,40,-1,48,-1,0,0,-1,-1,-1,-1,-1,
    10,-1,86,-1,62,-1,28,-1,85,-1,16,-1,34,-1,73,-1,-1,-1,0,0,-1,-1,-1,-1,
    -1,28,-1,32,-1,81,-1,27,-1,88,-1,5,-1,56,-1,37,-1,-1,-1,0,0,-1,-1,-1,
    23,-1,29,-1,15,-1,30,-1,66,-1,24,-1,50,-1,62,-1,-1,-1,-1,-1,0,0,-1,-1,
    -1,30,-1,65,-1,54,-1,14,-1,0,-1,30,-1,74,-1,0,-1,-1,-1,-1,-1,0,0,-1,
    32,-1,0,-1,15,-1,56,-1,85,-1,5,-1,6,-1,52,-1,0,-1,-1,-1,-1,-1,0,0,
    -1,0,-1,47,-1,13,-1,61,-1,84,-1,55,-1,78,-1,41,95,-1,-1,-1,-1,-1,-1,0
};
const char h_seed_3_4_a[]={
    6,38,3,93,-1,-1,-1,30,70,-1,86,-1,37,38,4,11,-1,46,48,0,-1,-1,-1,-1,62,94,19,84,-1,92,78,-1,15,-1,-1,92,-1,45,24,32,30,-1,-1,0,0,-1,-1,-1,71,-1,55,-1,12,66,45,79,-1,78,-1,-1,10,-1,22,55,70,82,-1,-1,0,0,-1,-1,38,61,-1,66,9,73,47,64,-1,39,61,43,-1,-1,-1,-1,95,32,0,-1,-1,0,0,-1,-1,-1,-1,-1,32,52,55,80,95,22,6,51,24,90,44,20,-1,-1,-1,-1,-1,-1,0,0,-1,63,31,88,20,-1,-1,-1,6,40,56,16,71,53,-1,-1,27,26,48,-1,-1,-1,-1,0
};
const char h_seed_3_4_b[]={
    -1,81,-1,28,-1,-1,14,25,17,-1,-1,85,29,52,78,95,22,92,0,0,-1,-1,-1,-1,42,-1,14,68,32,-1,-1,-1,-1,70,43,11,36,40,33,57,38,24,-1,0,0,-1,-1,-1,-1,-1,20,-1,-1,63,39,-1,70,67,-1,38,4,72,47,29,60,5,80,-1,0,0,-1,-1,64,2,-1,-1,63,-1,-1,3,51,-1,81,15,94,9,85,36,14,19,-1,-1,-1,0,0,-1,-1,53,60,80,-1,26,75,-1,-1,-1,-1,86,77,1,3,72,60,25,-1,-1,-1,-1,0,0,77,-1,-1,-1,15,28,-1,35,-1,72,30,68,85,84,26,64,11,89,0,-1,-1,-1,-1,0
};
const char h_seed_5_6[]={
    1,25,55,-1,47,4,-1,91,84,8,86,52,82,33,5,0,36,20,4,77,80,0,-1,-1,-1,6,-1,36,40,47,12,79,47,-1,41,21,12,71,14,72,0,44,49,0,0,0,0,-1,51,81,83,4,67,-1,21,-1,31,24,91,61,81,9,86,78,60,88,67,15,-1,-1,0,0,50,-1,50,15,-1,36,13,10,11,20,53,90,29,92,57,30,84,92,11,66,80,-1,-1,0
};

const size_t n_b=24;

matrix * ieee_ldpc_expand(const matrix* h_seed,const size_t z,const enum rate_type rate){
    matrix* matrix_expand=matrix_alloc(h_seed->row*z,h_seed->col*z);
    matrix_set_all(matrix_expand,0);
    int row,col,row_p,col_p;
    VAR permut;
    for(row=0;row<h_seed->row;row++){
        for(col=0;col<h_seed->col;col++){
            if((permut=matrix_get(h_seed,row,col))>=0){
                if(rate != rate_2_3_a){
                    permut=permut*z/96;
                    //printf("permut=%d\n",permut);
                    //printf("z=%d\n",z);
                }else{
                    permut=permut%z;
                }
                for(row_p=0;row_p<z;row_p++){
                    for(col_p=0;col_p<z;col_p++){
                        if((z+col_p-row_p)%z==permut){//
                            matrix_set(matrix_expand,row*z+row_p,col*z+col_p,1);
                        }
                    }
                }
            }
        }
    }
    return matrix_expand;
}

matrix * ieee_ldpc_encode(const matrix* matrix_k,const matrix* matrix_h){
    const int ldpc_n=matrix_h->col;
    const int ldpc_m=matrix_h->row;
    const int ldpc_k=ldpc_n-ldpc_m;
    int c;
    int z=ldpc_n/n_b;
    //printf("z=%d,ldpc_n=%d,ldpc_m=%d,ldpc_k=%d\n",z,ldpc_n,ldpc_m,ldpc_k);
    matrix* A,* B,*C,*D,*T,*E,*P,*P1,*P2;
    matrix *matrix_n=matrix_alloc(1,ldpc_n);
    A=matrix_sub(matrix_h,0,0,ldpc_m-z,ldpc_k);
    B=matrix_sub(matrix_h,0,ldpc_k,ldpc_m-z,z);
    C=matrix_sub(matrix_h,ldpc_m-z,0,z,ldpc_k);
    D=matrix_sub(matrix_h,ldpc_m-z,ldpc_k,z,z);
    T=matrix_sub(matrix_h,0,ldpc_k+z,ldpc_m-z,ldpc_m-z);
    E=matrix_sub(matrix_h,ldpc_m-z,ldpc_k+z,z,ldpc_m-z);

	//printf("ldpc_k=%d,z=%d,ldpc_m=%d,ldpc_n=%d",ldpc_k,z,ldpc_m,ldpc_n);
	//matrix_printf(T);
    matrix* tmp1=matrix_alloc(z,ldpc_m-z);//tmp1 z ldpc_m-z
    matrix* tmp2=matrix_alloc(z,ldpc_k);//tmp2 z*ldpc_k
    matrix* tmp4=matrix_alloc(z,1);//tmp4 z,1
    matrix* tmp5=matrix_alloc(ldpc_m-z,1);
    matrix* tmp6=matrix_alloc(ldpc_m-z,1);
    matrix* tmp7=matrix_alloc(ldpc_m-z,1);
    P1=matrix_alloc(1,z);
    P2=matrix_alloc(1,ldpc_m-z);

    matrix_mul(E,matrix_inverse(T),tmp1);
    matrix_mul(tmp1,A,tmp2);
    matrix_add(tmp2,C,tmp2);
    matrix_mul(tmp2,matrix_tran(matrix_k),tmp4);
    P1=matrix_tran(tmp4);
	/////////////////////////////////////
	//printf("A=\n");
	//matrix_printf(A);
	//printf("matrix_k=\n");
	//matrix_printf(matrix_k);
    matrix_mul(A,matrix_tran(matrix_k),tmp5);//Error in matrix_mul
	//printf("tmp5=\n");
	//matrix_printf(matrix_tran(tmp5));
    matrix_mul(A,matrix_tran(matrix_k),tmp5);//Error in matrix_mul
	//printf("tmp5cl=\n");
	//matrix_printf(matrix_tran(tmp5));
	////////////////////////////////////////
    matrix_mul(B,matrix_tran(P1),tmp6);
    matrix_add(tmp5,tmp6,tmp7);

    matrix_mul(matrix_inverse(T),tmp7,tmp5);
    P2=matrix_tran(tmp5);
    for(c=0;c<ldpc_n;c++){
        if(c<ldpc_k){
            matrix_set(matrix_n,0,c,mod2(matrix_get(matrix_k,0,c)));
        }else if (c<ldpc_k+z){
            matrix_set(matrix_n,0,c,mod2(matrix_get(P1,0,c-ldpc_k)));
        }else {
            matrix_set(matrix_n,0,c,mod2(matrix_get(P2,0,c-ldpc_k-z)));
        }

        // something should be added
    }
    return matrix_n;
}

matrix * ieee_ldpc_get_h_seed(enum rate_type rate){
    matrix * h_seed;
    switch(rate){
        case rate_1_2:
            h_seed=matrix_from_array(h_seed_1_2,12,n_b);
            break;
        case rate_2_3_a:
            h_seed=matrix_from_array(h_seed_2_3_a,8,n_b);
            break;
        case rate_2_3_b:
            h_seed=matrix_from_array(h_seed_2_3_b,8,n_b);
            break;
        case rate_3_4_a:
            h_seed=matrix_from_array(h_seed_3_4_a,6,n_b);
            break;
        case rate_3_4_b:
            h_seed=matrix_from_array(h_seed_3_4_b,6,n_b);
            break;
        case rate_5_6:
            h_seed=matrix_from_array(h_seed_5_6,4,n_b);
            break;
    }
    return h_seed;
}

matrix * ieee_ldpc_get_h(const size_t n,const enum rate_type rate){
    int z=n/n_b;
    matrix * matrix_h_seed = ieee_ldpc_get_h_seed(rate);
    matrix * matrix_h = ieee_ldpc_expand(matrix_h_seed,z,rate);
    return matrix_h;
}

/*
matrix *encode(matrix* matrix_k,matrix *matrix_h){
    int g;
    int c;
    int N=matrix_h->col;
    int M=matrix_h->row;
    int K=N-M;
    printf("N=%d,K=%d,M=%d\n",N,K,M);
    matrix* A,* B,*C,*D,*T,*E,*P,*P1,*P2;
    matrix *matrix_n=matrix_alloc(1,N);
    //g=M-matrix_l_change(matrix_h);
    g=24;
    if(g==0){
        A=matrix_sub(matrix_h,0,0,M-g,K);
        T=matrix_sub(matrix_h,0,K+g,M-g,M-g);
        matrix * tmp=matrix_alloc(M-g,K);
        matrix_mul(matrix_inverse(T),A,tmp);
        P=matrix_alloc(M-g,1);
        matrix_mul(tmp,matrix_tran(matrix_k),P);
        matrix_element_mul(P,-1,P);
        P=matrix_tran(P);
        int i=0;
        for(i=0;i<N;i++){
            if(i<K){
                matrix_set(matrix_n,0,i,matrix_get(matrix_k,0,i));
            }else{
                matrix_set(matrix_n,0,i,mod2(matrix_get(P,0,i-K)));
            }
        }
    }else{
        A=matrix_sub(matrix_h,0,0,M-g,K);
        B=matrix_sub(matrix_h,0,K,M-g,g);
        C=matrix_sub(matrix_h,M-g,0,g,K);
        D=matrix_sub(matrix_h,M-g,K,g,g);
        T=matrix_sub(matrix_h,0,K+g,M-g,M-g);
        E=matrix_sub(matrix_h,M-g,K+g,g,M-g);

        matrix* tmp1=matrix_alloc(g,M-g);//tmp1 g M-g
        matrix_mul(E,matrix_inverse(T),tmp1);
        matrix* tmp2=matrix_alloc(g,K);//tmp2 g*K
        matrix_mul(tmp1,A,tmp2);
        matrix_element_mul(tmp2,-1,tmp2);
        matrix_add(tmp2,C,tmp2);
        matrix* tmp3=matrix_alloc(g,g);//tmp3 g g
        matrix_mul(tmp1,B,tmp3);
        matrix_element_mul(tmp3,-1,tmp3);
        matrix_add(tmp3,D,tmp3);
        tmp3=matrix_inverse(tmp3);
        matrix_mul(tmp3,tmp2,tmp2);
        matrix* tmp4=matrix_alloc(g,1);//tmp4 g,1
        matrix_mul(tmp2,matrix_tran(matrix_k),tmp4);
        matrix_element_mul(tmp4,-1,tmp4);
        P1=matrix_alloc(1,g);
        P2=matrix_alloc(1,M-g);
        P1=matrix_tran(tmp4);
        matrix* tmp5=matrix_alloc(M-g,1);
        matrix* tmp6=matrix_alloc(M-g,1);
        matrix_mul(A,matrix_tran(matrix_k),tmp5);
        matrix_mul(B,matrix_tran(P1),tmp6);
        //matrix_printf(tmp5);
        //matrix_printf(tmp6);
        matrix_add(tmp5,tmp6,tmp5);
        matrix_mul(matrix_inverse(T),tmp5,tmp5);
        matrix_element_mul(tmp5,-1,tmp5);
        P2=matrix_tran(tmp5);
        for(c=0;c<N;c++){
            if(c<K){
                matrix_set(matrix_n,0,c,mod2(matrix_get(matrix_k,0,c)));
            }else if (c<K+g){
                matrix_set(matrix_n,0,c,mod2(matrix_get(P1,0,c-K)));
            }else {
                matrix_set(matrix_n,0,c,mod2(matrix_get(P2,0,c-K-g)));
            }
        }

        // something should be added
    }
    return matrix_n;
}
*/
