//
//  matrix_double.h
//  MyLdpc
//
//  Created by Wing on 4/26/15.
//  Copyright (c) 2015 Wing. All rights reserved.
//

#ifndef MyLdpc_matrix_double_h
#define MyLdpc_matrix_double_h
#define DIM 1
#include "cl_create.h"
//#include<gsl/gsl_matrix.h>
typedef struct{
    double* data;
    int row;
    int col;
}matrix_double;

matrix_double* matrix_double_alloc(int row,int col);

void matrix_double_set(matrix_double* mat,int r,int c, double var);

double matrix_double_get(const matrix_double* mat,int r,int c);

int matrix_double_mul(matrix_double* m1,matrix_double* m2, matrix_double* result);

matrix_double *matrix_double_from_array(const double *array,int row,int col);

matrix_double  * matrix_double_tran(const matrix_double* mat);

void matrix_double_printf(matrix_double *mat);

void matrix_double_printf_mod2(matrix_double *mat);

void matrix_double_row_add(matrix_double* mat_des,int row_des,matrix_double* mat_src,int row_src,int ratio);

void matrix_double_col_add(matrix_double* mat_des,int col_des,matrix_double* mat_src,int col_src,int ratio);

void matrix_double_row_set(matrix_double* mat,int row,matrix_double * block);

matrix_double * matrix_double_row_get(matrix_double*mat,int row);

void matrix_double_col_set(matrix_double* mat,int col,matrix_double * block);

matrix_double * matrix_double_col_get(matrix_double*mat,int col);

void matrix_double_swap_row(matrix_double* mat,int row1,int row2);

void matrix_double_swap_col(matrix_double* mat,int col1,int col2);

int matrix_double_l_change(matrix_double* mat);

matrix_double* matrix_double_inverse(matrix_double*);

matrix_double* matrix_double_sub(const matrix_double* mat,int row_offset,int col_offset,int row_size,int col_size);

void matrix_double_add(matrix_double* mat1,matrix_double* mat2,matrix_double* result);

void matrix_double_element_mul(matrix_double* mat_src,int ratio,matrix_double* mat_des);

void matrix_double_element_change(matrix_double* mat,int row,int col);

double matrix_double_mod2(double tmp);

//void gsl_matrix_double_printf(gsl_matrix* mat);

int matrix_double_equal(matrix_double* mat1,matrix_double* mat2);

void matrix_double_set_all(matrix_double* mat,double v);

double * matrix_double_to_array(matrix_double* mat);

void matrix_double_copy(matrix_double* src,matrix_double* des);
#endif
