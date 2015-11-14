//
//  matrix.h
//  MyLdpc
//
//  Created by Wing on 4/26/15.
//  Copyright (c) 2015 Wing. All rights reserved.
//

#ifndef MyLdpc_matrix_h
#define MyLdpc_matrix_h
#define VAR char
#define DIM 1
#include "cl_create.h"
typedef struct{
    VAR* data;
    int row;
    int col;
}matrix;

matrix* matrix_alloc(int row,int col);

void matrix_free(matrix* mat);

void matrix_set(matrix* mat,int r,int c, VAR var);

VAR matrix_get(const matrix* mat,int r,int c);

int matrix_mul(matrix* m1,matrix* m2, matrix* result);

int cl_matrix_mul(matrix* m1,matrix* m2, matrix* result);

matrix *matrix_from_array(const VAR *array,int row,int col);

matrix  * matrix_tran(const matrix* mat);

void matrix_printf(matrix *mat);

void matrix_printf_mod2(matrix *mat);

void matrix_row_add(matrix* mat_des,int row_des,matrix* mat_src,int row_src,int ratio);

void matrix_col_add(matrix* mat_des,int col_des,matrix* mat_src,int col_src,int ratio);

void matrix_row_set(matrix* mat,int row,matrix * block);

matrix * matrix_row_get(matrix*mat,int row);

void matrix_col_set(matrix* mat,int col,matrix * block);

matrix * matrix_col_get(matrix*mat,int col);

void matrix_swap_row(matrix* mat,int row1,int row2);

void matrix_swap_col(matrix* mat,int col1,int col2);

int matrix_l_change(matrix* mat);

matrix* matrix_inverse(matrix*);

matrix* matrix_sub(const matrix* mat,int row_offset,int col_offset,int row_size,int col_size);

void matrix_add(matrix* mat1,matrix* mat2,matrix* result);

void matrix_element_mul(matrix* mat_src,int ratio,matrix* mat_des);

void matrix_element_change(matrix* mat,int row,int col);

VAR mod2(VAR tmp);

//void gsl_matrix_printf(gsl_matrix* mat);

int matrix_equal(matrix* mat1,matrix* mat2);

void matrix_set_all(matrix* mat,VAR v);

VAR * matrix_to_array(matrix* mat);

void matrix_copy(matrix* src,matrix* des);
#endif
