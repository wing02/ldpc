#include"matrix.h"

enum rate_type{rate_1_2,rate_2_3_a,rate_2_3_b,rate_3_4_a,rate_3_4_b,rate_5_6};

matrix * ieee_ldpc_expand(const matrix* h_seed,const size_t z,const enum rate_type rate);

matrix * ieee_ldpc_encode(const matrix* h,const matrix* matrix_k);

matrix * ieee_ldpc_get_h_seed(enum rate_type rate);

matrix * ieee_ldpc_get_h(const size_t z,const enum rate_type rate);

matrix *encode(matrix* matrix_k,matrix *matrix_h);
