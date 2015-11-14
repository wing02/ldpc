#include"matrix.h"
#include<stdio.h>

int matrix_mul(matrix *m1,matrix* m2,matrix * result) {
    if(m1->row!=result->row || m1->col!=m2->row ||m2->col!=result->col){
        printf("Error, matrix_mul\n");
        exit(1);
    }
    int r,c,i;
    VAR tmp;
    matrix * result_tmp=matrix_alloc(result->row,result->col);
    for(r=0;r<(result->row);r++){
        for(c=0;c<(result->col);c++){
            tmp=0;
            for(i=0;i<(m1->col);i++){
                tmp+=matrix_get(m1,r,i)*matrix_get(m2,i,c);
            }
            matrix_set(result_tmp,r,c,tmp);
        }
    }
    for(r=0;r<(result->row);r++){
        for(c=0;c<(result->col);c++){
            matrix_set(result,r,c,matrix_get(result_tmp,r,c));
        }
    }
}
int cl_matrix_mul(matrix *m1,matrix* m2,matrix * result) {
    cl_device_type device_type=CL_DEVICE_TYPE_GPU;
    if(m1->row!=result->row || m1->col!=m2->row ||m2->col!=result->col){
        printf("Error, matrix_mul\n");
        exit(1);
    }

    VAR *A=m1->data;//N*P
    VAR *B=m2->data;//P*M
    VAR *C=result->data;//N*M
    int Mdim=m2->col;
    int Ndim=m1->row;
    int Pdim=m2->row;
	printf("M=%d,N=%d,P=%d,\n",Mdim,Ndim,Pdim);
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
	a_in = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(VAR ) * szA, NULL,
	NULL);
	b_in = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(VAR ) * szB, NULL,
	NULL);
	c_out = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(VAR ) * szC,
	NULL, NULL);


//set the program deault
	program=create_program(context,device_id,"./matrix_mul_cl.c");

	//kernel
	kernel = clCreateKernel(program, "matrix_mul", &err);
	err = 0;
	err = clSetKernelArg(kernel, 0, sizeof(int), &Mdim);
	err |= clSetKernelArg(kernel, 1, sizeof(int), &Ndim);
	err |= clSetKernelArg(kernel, 2, sizeof(int), &Pdim);
	err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &a_in);
	err |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &b_in);
	err |= clSetKernelArg(kernel, 5, sizeof(cl_mem), &c_out);
	//err |= clSetKernelArg(kernel, 6, sizeof(VAR )*Pdim, NULL);
	
	err = clEnqueueWriteBuffer(commands, a_in, CL_TRUE, 0, sizeof(VAR ) * szA,
			A, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(commands, b_in, CL_TRUE, 0, sizeof(VAR ) * szB,
			B, 0, NULL, NULL);
	cl_event prof_event;

	cl_uint cl_device_max_compute_units;
	size_t size;
	err |= clGetDeviceInfo(device_id,CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(cl_uint),&cl_device_max_compute_units,&size);
	//printf("The max compute units is %d.\n",cl_device_max_compute_units);
	global[0] = (size_t) Ndim;
	local[0]=(size_t)Ndim/cl_device_max_compute_units;
	local[0]=(size_t)Ndim/8;
	nd = DIM;
	//printf("global=%d\n",global[0]);
	err = clEnqueueNDRangeKernel(commands, kernel, nd, NULL, global, local, 0,
	NULL, &prof_event);
	clFinish(commands);

	cl_ulong ev_start_time = (cl_ulong) 0;
	cl_ulong ev_end_time = (cl_ulong) 0;

	err = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_START,sizeof(cl_ulong), &ev_start_time, NULL);
	err = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END,sizeof(cl_ulong), &ev_end_time, NULL);
	//printf("time:%lldms\n",(ev_end_time-ev_start_time)/1000000);
    //printf("time:%fms\n",(float)(ev_end_time-ev_start_time)/1000000);
	err = clEnqueueReadBuffer(commands, c_out, CL_TRUE, 0, sizeof(VAR ) * szC,
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

matrix* matrix_alloc(int row,int col){
    matrix* tmp=(matrix*)malloc(sizeof(matrix));
    tmp->row=row;
    tmp->col=col;
    tmp->data=(VAR*)malloc(sizeof(VAR)*row*col);
    return tmp;
}

void matrix_free(matrix* mat){
    free(mat->data);
    free(mat);
}

void matrix_set(matrix* mat,int r,int c, VAR var){
    mat->data[(mat->col) * r +c]=var;
}
VAR matrix_get(const matrix* mat,int r,int c){
    return mat->data[(mat->col)*r+c];
}
matrix * matrix_from_array(const VAR *array,int row,int col){
    matrix *tmp=matrix_alloc(row,col);
    int i;
    for(i=0;i<row*col;i++){
        matrix_set(tmp,(i/col),(i%col),array[i]);
    }
    return tmp;
}
matrix * matrix_tran(const matrix* src){
    matrix * tmp=matrix_alloc(src->col,src->row);
    int r,c;
    for(r=0;r<tmp->row;r++){
        for(c=0;c<tmp->col;c++){
            matrix_set(tmp,r,c,matrix_get(src,c,r));
        }
    }
    return tmp;
}

void matrix_printf(matrix *mat){
    int r,c;
    for(r=0;r<mat->row;r++){
        for(c=0;c<mat->col;c++){
            printf("%d ",matrix_get(mat,r,c));
        }
        printf("\n");
    }
    printf("\n");
    
}

void matrix_printf_mod2(matrix *mat){
    int r,c;
    for(r=0;r<mat->row;r++){
        for(c=0;c<mat->col;c++){
            printf("%d ",mod2(matrix_get(mat,r,c)));
        }
        printf("\n");
    }
    
}
void matrix_row_add(matrix* mat_des,int row_des,matrix* mat_src,int row_src,int ratio){
    if(mat_des->col!=mat_src->col){
        printf("Error,the col is different!");
        exit(0);
    }
    int i;
    for(i=0;i<mat_src->col;i++){
        // des=des+src*ratio
        matrix_set(mat_des,row_des,i,(matrix_get(mat_des,row_des,i)+ratio*matrix_get(mat_src,row_src,i)));
    }
}
void matrix_col_add(matrix* mat_des,int col_des,matrix* mat_src,int col_src,int ratio){
    if(mat_des->row!=mat_src->row){
        printf("Error,the row is different!");
        exit(0);
    }
    int i;
    for(i=0;i<mat_src->row;i++){
        // des=des+src*ratio
        matrix_set(mat_des,i,col_des,(matrix_get(mat_des,i,col_des)+ratio*matrix_get(mat_src,i,col_src)));
    }
}

void matrix_row_set(matrix* mat,int row,matrix * block){
    if(block->row!=1 || block->col!=mat->col){
        printf("Error,the block size is not fit");
    }
    int i;
    for(i=0;i<mat->col;i++){
        matrix_set(mat,row,i,matrix_get(block,0,i));
    }
}

matrix * matrix_row_get(matrix*mat,int row){
    matrix* tmp=matrix_alloc(1,mat->col);
    int i=1;
    for(i=0;i<mat->col;i++){
        matrix_set(tmp,0,i,matrix_get(mat,row,i));
    }
    return tmp;
}

void matrix_col_set(matrix* mat,int col,matrix * block){
    if(block->col!=1 || block->row!=mat->row){
        printf("Error,the block size is not fit");
    }
    int i;
    for(i=0;i<mat->row;i++){
        matrix_set(mat,i,col,matrix_get(block,i,0));
    }
}
matrix * matrix_col_get(matrix*mat,int col){
    matrix* tmp=matrix_alloc(mat->row,1);
    int i =1;
    for(i=0;i<mat->row;i++){
        matrix_set(tmp,i,0,matrix_get(mat,i,col));
    }
    return tmp;
}

void matrix_swap_row(matrix* mat,int row1,int row2){
    int i;
    for(i=0;i<mat->col;i++){
        VAR tmp=matrix_get(mat,row1,i);
        matrix_set(mat,row1,i,matrix_get(mat,row2,i));
        matrix_set(mat,row2,i,tmp);
    }
}
void matrix_swap_col(matrix* mat,int col1,int col2){
    int i;
    for(i=0;i<mat->row;i++){
        VAR tmp=matrix_get(mat,i,col1);
        matrix_set(mat,i,col1,matrix_get(mat,i,col2));
        matrix_set(mat,i,col2,tmp);
    }
}
void matrix_num_swap(int *s1,int *s2){
    int tmp=*s1;
    *s1=*s2;
    *s2=tmp;
}
int matrix_l_change(matrix* mat){
    int row_start=0,col_start=0,row_end=mat->row,col_end=mat->col;
    int * row_heavy=(int *)malloc(sizeof(int)*mat->row);
    int r,c;
    for(r=0;r<mat->row;r++){
        row_heavy[r]=0;
        for(c=0;c<mat->col;c++){
            row_heavy[r]+=matrix_get(mat,r,c);
        }
    }
    while(row_end>row_start && col_end>col_start){
        int found=0;
        for(r=row_start;r<row_end;r++){
            if(row_heavy[r]==1){
                matrix_swap_row(mat,r,row_start);
                matrix_num_swap(&row_heavy[r],&row_heavy[row_start]);
                for(c=col_start;c<col_end;c++){
                    if(matrix_get(mat,row_start,c)==1){
                        matrix_swap_col(mat,c,col_start);
                        break;
                    }
                }
                //log
                //printf("1!!change row %f and %f\n",r,row_start);
                //printf("1!!change col %f and %f\n",c,col_start);
                row_start++;
                col_start++;
                for(r=row_start;r<row_end;r++){
                    if(matrix_get(mat,r,col_start-1)==1){
                        row_heavy[r]-=1;
                    }
                }
                found=1;
                break;
            }else if(row_heavy[r]==0){
                row_end--;
                if(r!=row_end){
                    matrix_swap_row(mat,r,row_end);
                    matrix_num_swap(&row_heavy[r],&row_heavy[r]);
                }
            }
        }
        if(found==0&&row_end>row_start&&col_end>col_start){
            col_end--;
            for(r=row_start;r<row_end;r++){
                if(matrix_get(mat,r,col_end)==1){
                    row_heavy[r]-=1;
                }
            }
        }
            for(r=0;r<mat->row;r++){
            }
    }
    for(c=row_start-1;c>=0;c--){
        matrix_swap_col(mat,c,mat->col-row_start+c);
    }
    return row_start;
}
matrix* matrix_inverse( matrix* mat_src){
    if(mat_src->row!=mat_src->col){
        printf("Error,row != col, no inverse");
        exit(0);
    }
    int r,r2,c,c2,tmp,size=mat_src->row;

    matrix* inv=matrix_alloc(size,size);
    matrix* mat=matrix_alloc(size,size);
    for(r=0;r<size;r++){
        for(c=0;c<size;c++){
            matrix_set(mat,r,c,matrix_get(mat_src,r,c));
            if(r==c){
                matrix_set(inv,r,c,1);
            }else{
                matrix_set(inv,r,c,0);
            }
        }
    }
    for(c=0;c<size;c++){
        for(r=c;r<size;r++){
            if(matrix_get(mat,r,c)!=0){
                tmp=matrix_get(mat,r,c);
                matrix_col_add(mat,c,mat,c,1/tmp-1);
                matrix_col_add(inv,c,inv,c,1/tmp-1);
                for(c2=c+1;c2<size;c2++){
                    matrix_col_add(mat,c2,mat,c,-matrix_get(mat,r,c2));
                    matrix_col_add(inv,c2,inv,c,-matrix_get(mat,r,c2));
                }
                break;
            }
        }
    }
    for(r=0;r<size;r++){
        for(r2=r+1;r2<size;r2++){
            int tmp=matrix_get(mat,r2,r);
            matrix_row_add(mat,r2,mat,r,-tmp);
            matrix_row_add(inv,r2,inv,r,-tmp);
        }
    }
    return inv;
}
matrix* matrix_sub(const matrix* mat,int row_offset,int col_offset,int row_size,int col_size){
    matrix* tmp=matrix_alloc(row_size,col_size);
    int r,c;
    for(r=0;r<row_size;r++){
        for(c=0;c<col_size;c++){
            matrix_set(tmp,r,c,matrix_get(mat,r+row_offset,c+col_offset));
        }
    }
    return tmp;
}

void matrix_add(matrix* mat1,matrix* mat2,matrix* result){
    int r,c;
    for(r=0;r<result->row;r++){
        for(c=0;c<result->col;c++){
            matrix_set(result,r,c,matrix_get(mat1,r,c)+matrix_get(mat2,r,c));
        }
    }
}

void matrix_element_mul(matrix* mat_src,int ratio,matrix* mat_des){
    int r,c;
    for(r=0;r<mat_src->row;r++){
        for(c=0;c<mat_src->col;c++){
            matrix_set(mat_des,r,c,matrix_get(mat_src,r,c)*ratio);
        }
    }
    
}

void matrix_element_change(matrix* mat,int row,int col){
    int element=matrix_get(mat,row,col);
    if(element==1 || element==0){
        matrix_set(mat,row,col,1-element);
    }else{
        printf("The element is not 0 or 1!");
        exit(0);
    }
}

VAR mod2(VAR tmp){
    if(tmp<0){
        tmp=-tmp;
    }
    tmp=tmp%2;
    return tmp;
}
//void gsl_matrix_printf(gsl_matrix* mat){
//    int r,c;
//    printf("\n******\n");
//    for(r=0;r<mat->size1;r++){
//        for(c=0;c<mat->size2;c++){
//            printf("%f ",(int)gsl_matrix_get(mat,r,c));
//        }
//        printf("\n");
//    }
//    printf("\n");
//}

int matrix_equal(matrix* mat1,matrix* mat2){
    int r,c;
    if(mat1->row!=mat2->row || mat1->col!=mat2->col){
        return 0;
    }
    for(r=0;r<mat1->row;r++){
        for(c=0;c<mat1->col;c++){
            if(matrix_get(mat1,r,c)!=matrix_get(mat2,r,c)){
                return 0;
            }
        }
    }
    return 1;
}

void matrix_set_all(matrix* mat,VAR v){
    int r,c;
    for(r=0;r<mat->row;r++){
        for(c=0;c<mat->col;c++){
            matrix_set(mat,r,c,v);
        }
    }
}

VAR * matrix_to_array(matrix* mat){
    int r,c;
    VAR * array=(VAR*)malloc(sizeof(VAR)*(mat->row)*(mat->col));
    for(r=0;r<mat->row;r++){
        for(c=0;c<mat->col;c++){
            array[r*(mat->col)+c]=matrix_get(mat,r,c);
        }
    }
    return array;
}

void matrix_copy(matrix* src,matrix* des){
    if(src->row!=des->row || src->col!=des->col){
        printf("Error in matrix_copy");
        exit(1);
    }
    int r,c;
    for(r=0;r<src->row;r++){
        for(c=0;c<src->col;c++){
            matrix_set(des,r,c,matrix_get(src,r,c));
        }
    }
}
