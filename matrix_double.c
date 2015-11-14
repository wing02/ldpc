#include"matrix_double.h"
#include<stdio.h>

/*
int matrix_double_mul(matrix_double *m1,matrix_double* m2,matrix_double * result) {
    if(m1->row!=result->row || m1->col!=m2->row ||m2->col!=result->col){
        printf("Error, matrix_double_mul\n");
        exit(1);
    }
    int r,c,i;
    double tmp;
    matrix_double * result_tmp=matrix_double_alloc(result->row,result->col);
    for(r=0;r<(result->row);r++){
        for(c=0;c<(result->col);c++){
            tmp=0;
            for(i=0;i<(m1->col);i++){
                tmp+=matrix_double_get(m1,r,i)*matrix_double_get(m2,i,c);
            }
            matrix_double_set(result_tmp,r,c,tmp);
        }
    }
    for(r=0;r<(result->row);r++){
        for(c=0;c<(result->col);c++){
            matrix_double_set(result,r,c,matrix_double_get(result_tmp,r,c));
        }
    }
}
*/
int matrix_double_mul(matrix_double*m1,matrix_double* m2,matrix_double* result) {
    cl_device_type device_type=CL_DEVICE_TYPE_GPU;
    double *A=m1->data;
    double *B=m2->data;
    double *C=result->data;
    int Mdim=m2->col;
    int Ndim=m1->row;
    int Pdim=m2->row;
	int err;
	int szA, szB, szC;
	size_t global[DIM];
	size_t local[DIM];
	cl_device_id device_id;
	cl_context context;
	cl_command_queue commands;
	cl_program program;
	cl_kernel kernel;
	cl_uint nd;
	cl_mem a_in;
	cl_mem b_in;
	cl_mem c_out;

	context = create_context(device_type);
	commands = create_command_queue(context, &device_id);

	szA = Ndim * Pdim;
	szB = Pdim * Mdim;
	szC = Ndim * Mdim;

	//initmat
	a_in = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(double ) * szA, NULL,
	NULL);
	b_in = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(double ) * szB, NULL,
	NULL);
	c_out = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(double ) * szC,
	NULL, NULL);


//set the program deault
	program=create_program(context,device_id,"./matrix_double_mul.cl");

	//kernel
	kernel = clCreateKernel(program, "matrix_double_mul", &err);
	err = 0;
	err = clSetKernelArg(kernel, 0, sizeof(int), &Mdim);
	err |= clSetKernelArg(kernel, 1, sizeof(int), &Ndim);
	err |= clSetKernelArg(kernel, 2, sizeof(int), &Pdim);
	err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &a_in);
	err |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &b_in);
	err |= clSetKernelArg(kernel, 5, sizeof(cl_mem), &c_out);
	err |= clSetKernelArg(kernel, 6, sizeof(double )*Pdim, NULL);
	
	err = clEnqueueWriteBuffer(commands, a_in, CL_TRUE, 0, sizeof(double ) * szA,
			A, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(commands, b_in, CL_TRUE, 0, sizeof(double ) * szB,
			B, 0, NULL, NULL);
	cl_event prof_event;

	global[0] = (size_t) Ndim;
	local[0]=(size_t)Ndim;
	nd = DIM;
	err = clEnqueueNDRangeKernel(commands, kernel, nd, NULL, global, local, 0,
	NULL, &prof_event);
	clFinish(commands);

	cl_ulong ev_start_time = (cl_ulong) 0;
	cl_ulong ev_end_time = (cl_ulong) 0;

	err = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_START,sizeof(cl_ulong), &ev_start_time, NULL);
	err = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END,sizeof(cl_ulong), &ev_end_time, NULL);
	//printf("time:%lldms\n",(ev_end_time-ev_start_time)/1000000);
    //printf("time:%fms\n",(float)(ev_end_time-ev_start_time)/1000000);
	err = clEnqueueReadBuffer(commands, c_out, CL_TRUE, 0, sizeof(double ) * szC,
			C, 0, NULL, NULL);

	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseMemObject(a_in);
	clReleaseMemObject(b_in);
	clReleaseMemObject(c_out);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
	return 0;
}

matrix_double* matrix_double_alloc(int row,int col){
    matrix_double* tmp=(matrix_double*)malloc(sizeof(matrix_double));
    tmp->row=row;
    tmp->col=col;
    tmp->data=(double*)malloc(sizeof(double)*row*col);
    return tmp;
}
void matrix_double_set(matrix_double* mat,int r,int c, double var){
    mat->data[(mat->col) * r +c]=var;
}
double matrix_double_get(const matrix_double* mat,int r,int c){
    return mat->data[(mat->col)*r+c];
}
matrix_double * matrix_double_from_array(const double *array,int row,int col){
    matrix_double *tmp=matrix_double_alloc(row,col);
    int i;
    for(i=0;i<row*col;i++){
        matrix_double_set(tmp,(i/col),(i%col),array[i]);
    }
    return tmp;
}
matrix_double * matrix_double_tran(const matrix_double* src){
    matrix_double * tmp=matrix_double_alloc(src->col,src->row);
    int r,c;
    for(r=0;r<tmp->row;r++){
        for(c=0;c<tmp->col;c++){
            matrix_double_set(tmp,r,c,matrix_double_get(src,c,r));
        }
    }
    return tmp;
}

void matrix_double_printf(matrix_double *mat){
    int r,c;
    for(r=0;r<mat->row;r++){
        for(c=0;c<mat->col;c++){
            printf("%f ",matrix_double_get(mat,r,c));
        }
        printf("\n");
    }
    printf("\n");
    
}

void matrix_double_printf_mod2(matrix_double *mat){
    int r,c;
    for(r=0;r<mat->row;r++){
        for(c=0;c<mat->col;c++){
            printf("%f ",matrix_double_mod2(matrix_double_get(mat,r,c)));
        }
        printf("\n");
    }
    
}
void matrix_double_row_add(matrix_double* mat_des,int row_des,matrix_double* mat_src,int row_src,int ratio){
    if(mat_des->col!=mat_src->col){
        printf("Error,the col is different!");
        exit(0);
    }
    int i;
    for(i=0;i<mat_src->col;i++){
        // des=des+src*ratio
        matrix_double_set(mat_des,row_des,i,(matrix_double_get(mat_des,row_des,i)+ratio*matrix_double_get(mat_src,row_src,i)));
    }
}
void matrix_double_col_add(matrix_double* mat_des,int col_des,matrix_double* mat_src,int col_src,int ratio){
    if(mat_des->row!=mat_src->row){
        printf("Error,the row is different!");
        exit(0);
    }
    int i;
    for(i=0;i<mat_src->row;i++){
        // des=des+src*ratio
        matrix_double_set(mat_des,i,col_des,(matrix_double_get(mat_des,i,col_des)+ratio*matrix_double_get(mat_src,i,col_src)));
    }
}

void matrix_double_row_set(matrix_double* mat,int row,matrix_double * block){
    if(block->row!=1 || block->col!=mat->col){
        printf("Error,the block size is not fit");
    }
    int i;
    for(i=0;i<mat->col;i++){
        matrix_double_set(mat,row,i,matrix_double_get(block,0,i));
    }
}

matrix_double * matrix_double_row_get(matrix_double*mat,int row){
    matrix_double* tmp=matrix_double_alloc(1,mat->col);
    int i=1;
    for(i=0;i<mat->col;i++){
        matrix_double_set(tmp,0,i,matrix_double_get(mat,row,i));
    }
    return tmp;
}

void matrix_double_col_set(matrix_double* mat,int col,matrix_double * block){
    if(block->col!=1 || block->row!=mat->row){
        printf("Error,the block size is not fit");
    }
    int i;
    for(i=0;i<mat->row;i++){
        matrix_double_set(mat,i,col,matrix_double_get(block,i,0));
    }
}
matrix_double * matrix_double_col_get(matrix_double*mat,int col){
    matrix_double* tmp=matrix_double_alloc(mat->row,1);
    int i =1;
    for(i=0;i<mat->row;i++){
        matrix_double_set(tmp,i,0,matrix_double_get(mat,i,col));
    }
    return tmp;
}

void matrix_double_swap_row(matrix_double* mat,int row1,int row2){
    int i;
    for(i=0;i<mat->col;i++){
        double tmp=matrix_double_get(mat,row1,i);
        matrix_double_set(mat,row1,i,matrix_double_get(mat,row2,i));
        matrix_double_set(mat,row2,i,tmp);
    }
}
void matrix_double_swap_col(matrix_double* mat,int col1,int col2){
    int i;
    for(i=0;i<mat->row;i++){
        double tmp=matrix_double_get(mat,i,col1);
        matrix_double_set(mat,i,col1,matrix_double_get(mat,i,col2));
        matrix_double_set(mat,i,col2,tmp);
    }
}
void matrix_double_num_swap(int *s1,int *s2){
    int tmp=*s1;
    *s1=*s2;
    *s2=tmp;
}
int matrix_double_l_change(matrix_double* mat){
    int row_start=0,col_start=0,row_end=mat->row,col_end=mat->col;
    int * row_heavy=(int *)malloc(sizeof(int)*mat->row);
    int r,c;
    for(r=0;r<mat->row;r++){
        row_heavy[r]=0;
        for(c=0;c<mat->col;c++){
            row_heavy[r]+=matrix_double_get(mat,r,c);
        }
    }
    while(row_end>row_start && col_end>col_start){
        int found=0;
        for(r=row_start;r<row_end;r++){
            if(row_heavy[r]==1){
                matrix_double_swap_row(mat,r,row_start);
                matrix_double_num_swap(&row_heavy[r],&row_heavy[row_start]);
                for(c=col_start;c<col_end;c++){
                    if(matrix_double_get(mat,row_start,c)==1){
                        matrix_double_swap_col(mat,c,col_start);
                        break;
                    }
                }
                //log
                //printf("1!!change row %f and %f\n",r,row_start);
                //printf("1!!change col %f and %f\n",c,col_start);
                row_start++;
                col_start++;
                for(r=row_start;r<row_end;r++){
                    if(matrix_double_get(mat,r,col_start-1)==1){
                        row_heavy[r]-=1;
                    }
                }
                found=1;
                break;
            }else if(row_heavy[r]==0){
                row_end--;
                if(r!=row_end){
                    matrix_double_swap_row(mat,r,row_end);
                    matrix_double_num_swap(&row_heavy[r],&row_heavy[r]);
                }
            }
        }
        if(found==0&&row_end>row_start&&col_end>col_start){
            col_end--;
            for(r=row_start;r<row_end;r++){
                if(matrix_double_get(mat,r,col_end)==1){
                    row_heavy[r]-=1;
                }
            }
        }
            for(r=0;r<mat->row;r++){
            }
    }
    for(c=row_start-1;c>=0;c--){
        matrix_double_swap_col(mat,c,mat->col-row_start+c);
    }
    return row_start;
}
matrix_double* matrix_double_inverse( matrix_double* mat_src){
    if(mat_src->row!=mat_src->col){
        printf("Error,row != col, no inverse");
        exit(0);
    }
    int r,r2,c,c2,tmp,size=mat_src->row;

    matrix_double* inv=matrix_double_alloc(size,size);
    matrix_double* mat=matrix_double_alloc(size,size);
    for(r=0;r<size;r++){
        for(c=0;c<size;c++){
            matrix_double_set(mat,r,c,matrix_double_get(mat_src,r,c));
            if(r==c){
                matrix_double_set(inv,r,c,1);
            }else{
                matrix_double_set(inv,r,c,0);
            }
        }
    }
    for(c=0;c<size;c++){
        for(r=c;r<size;r++){
            if(matrix_double_get(mat,r,c)!=0){
                tmp=matrix_double_get(mat,r,c);
                matrix_double_col_add(mat,c,mat,c,1/tmp-1);
                matrix_double_col_add(inv,c,inv,c,1/tmp-1);
                for(c2=c+1;c2<size;c2++){
                    matrix_double_col_add(mat,c2,mat,c,-matrix_double_get(mat,r,c2));
                    matrix_double_col_add(inv,c2,inv,c,-matrix_double_get(mat,r,c2));
                }
                break;
            }
        }
    }
    for(r=0;r<size;r++){
        for(r2=r+1;r2<size;r2++){
            int tmp=matrix_double_get(mat,r2,r);
            matrix_double_row_add(mat,r2,mat,r,-tmp);
            matrix_double_row_add(inv,r2,inv,r,-tmp);
        }
    }
    return inv;
}
matrix_double* matrix_double_sub(const matrix_double* mat,int row_offset,int col_offset,int row_size,int col_size){
    matrix_double* tmp=matrix_double_alloc(row_size,col_size);
    int r,c;
    for(r=0;r<row_size;r++){
        for(c=0;c<col_size;c++){
            matrix_double_set(tmp,r,c,matrix_double_get(mat,r+row_offset,c+col_offset));
        }
    }
    return tmp;
}

void matrix_double_add(matrix_double* mat1,matrix_double* mat2,matrix_double* result){
    int r,c;
    for(r=0;r<result->row;r++){
        for(c=0;c<result->col;c++){
            matrix_double_set(result,r,c,matrix_double_get(mat1,r,c)+matrix_double_get(mat2,r,c));
        }
    }
}

void matrix_double_element_mul(matrix_double* mat_src,int ratio,matrix_double* mat_des){
    int r,c;
    for(r=0;r<mat_src->row;r++){
        for(c=0;c<mat_src->col;c++){
            matrix_double_set(mat_des,r,c,matrix_double_get(mat_src,r,c)*ratio);
        }
    }
    
}

void matrix_double_element_change(matrix_double* mat,int row,int col){
    int element=matrix_double_get(mat,row,col);
    if(element==1 || element==0){
        matrix_double_set(mat,row,col,1-element);
    }else{
        printf("The element is not 0 or 1!");
        exit(0);
    }
}

double matrix_double_mod2(double tmp){
    if(tmp<0){
        tmp=-tmp;
    }
    tmp=(int)tmp%2;
    return tmp;
}
//void gsl_matrix_double_printf(gsl_matrix* mat){
//    int r,c;
//    printf("\n******\n");
//    for(r=0;r<mat->size1;r++){
//        for(c=0;c<mat->size2;c++){
//            printf("%f ",(int)gsl_matrix_double_get(mat,r,c));
//        }
//        printf("\n");
//    }
//    printf("\n");
//}

int matrix_double_equal(matrix_double* mat1,matrix_double* mat2){
    int r,c;
    if(mat1->row!=mat2->row || mat1->col!=mat2->col){
        return 0;
    }
    for(r=0;r<mat1->row;r++){
        for(c=0;c<mat1->col;c++){
            if(matrix_double_get(mat1,r,c)!=matrix_double_get(mat2,r,c)){
                return 0;
            }
        }
    }
    return 1;
}

void matrix_double_set_all(matrix_double* mat,double v){
    int r,c;
    for(r=0;r<mat->row;r++){
        for(c=0;c<mat->col;c++){
            matrix_double_set(mat,r,c,v);
        }
    }
}

double * matrix_double_to_array(matrix_double* mat){
    int r,c;
    double * array=(double*)malloc(sizeof(double)*(mat->row)*(mat->col));
    for(r=0;r<mat->row;r++){
        for(c=0;c<mat->col;c++){
            array[r*(mat->col)+c]=matrix_double_get(mat,r,c);
        }
    }
    return array;
}

void matrix_double_copy(matrix_double* src,matrix_double* des){
    if(src->row!=des->row || src->col!=des->col){
        printf("Error in matrix_double_copy");
        exit(1);
    }
    int r,c;
    for(r=0;r<src->row;r++){
        for(c=0;c<src->col;c++){
            matrix_double_set(des,r,c,matrix_double_get(src,r,c));
        }
    }
}
