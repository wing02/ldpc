//
//  decode.c
//  MyLdpc
//
//  Created by Wing on 4/29/15.
//  Copyright (c) 2015 Wing. All rights reserved.
//

#include "ieee_ldpc_decode.h"
#include"matrix_double.h"
#include<math.h>
#define ZERO 0
#define TIME 30


int decode(matrix *matrix_n,matrix* matrix_h){
    int flag;
    const int ldpc_n=matrix_n->col;
    const int ldpc_m=matrix_h->row;
    const int ldpc_k=ldpc_n-ldpc_m;
    
    //if no loss;
    flag=check_result(matrix_n,matrix_h);
    if(flag==1)
        return 0;
    int c,r;
    double tmp0,tmp1,tmp;
    VAR *n=(VAR*)malloc(sizeof(VAR)*ldpc_n);
    for(c=0;c<ldpc_n;c++){
        n[c]=matrix_get(matrix_n,0,c);
    }
    double *prior_p0=(double*)malloc(sizeof(double)*ldpc_n);
    double *prior_p1=(double*)malloc(sizeof(double)*ldpc_n);
    matrix_double *r0=matrix_double_alloc(ldpc_m,ldpc_n);
    matrix_double *r1=matrix_double_alloc(ldpc_m,ldpc_n);
    matrix_double *q0=matrix_double_alloc(ldpc_m,ldpc_n);
    matrix_double *q1=matrix_double_alloc(ldpc_m,ldpc_n);
    for(r=0;r<ldpc_m;r++){
        for(c=0;c<ldpc_n;c++){
            matrix_double_set(r0,r,c,0);
            matrix_double_set(r1,r,c,0);
            matrix_double_set(q0,r,c,0);
            matrix_double_set(q1,r,c,0);
        }
    }
    //init
    for(c=0;c<ldpc_n;c++){
        VAR element=matrix_get(matrix_n,0,c);
        if(element==0){
            prior_p0[c]=1;
            prior_p1[c]=0;
            for(r=0;r<ldpc_m;r++){
                matrix_double_set(q0,r,c,1);
                matrix_double_set(q1,r,c,0);
            }
        }else if(element==1){
            prior_p0[c]=0;
            prior_p1[c]=1;
            for(r=0;r<ldpc_m;r++){
                matrix_double_set(q0,r,c,0);
                matrix_double_set(q1,r,c,1);
            }
        }else if(element==ERR){
            prior_p0[c]=0.5;
            prior_p1[c]=0.5;
            for(r=0;r<ldpc_m;r++){
                matrix_double_set(q0,r,c,0.5);
                matrix_double_set(q1,r,c,0.5);
            }
        }
    }
    int time=TIME;
    while(time--){
        //refresh r0 r1
        for(r=0;r<ldpc_m;r++){
            for(c=0;c<ldpc_n ;c++){
                if(matrix_get(matrix_h,r,c)==0)
                    continue;
                int cc;//cc means l'
                tmp=1;
                for(cc=0;cc<ldpc_n;cc++){
                    if(cc!=c && matrix_get(matrix_h,r,cc)==1 ){
                        tmp*=matrix_double_get(q0,r,cc)-matrix_double_get(q1,r,cc);
                    }
                }
                matrix_double_set(r0,r,c,(1+tmp)/2);
                matrix_double_set(r1,r,c,(1-tmp)/2);
            }
        }
        //refresh L_q
        for(r=0;r<ldpc_m;r++){
            for(c=0;c<ldpc_n;c++){
                if(matrix_get(matrix_h,r,c)==0)
                    continue;
                tmp0=prior_p0[c];
                tmp1=prior_p1[c];
                int rr;//rr means m'
                for(rr=0;rr<ldpc_m;rr++){
                    if(rr!=r && matrix_get(matrix_h,rr,c)==1){
                        tmp0*=matrix_double_get(r0,rr,c);
                        tmp1*=matrix_double_get(r1,rr,c);
                    }
                }
                tmp=tmp0+tmp1;
                tmp0/=tmp;
                tmp1/=tmp;
                matrix_double_set(q0,r,c,tmp0);
                matrix_double_set(q1,r,c,tmp1);
            }
        }
        //refresh L_posterior_ip
        //matrix_double_printf(r1);
        for(c=0;c<ldpc_n;c++){
            tmp0=prior_p0[c];
            tmp1=prior_p1[c];
            for(r=0;r<ldpc_m;r++){
                if(matrix_get(matrix_h,r,c)==1){
                    tmp0*=matrix_double_get(r0,r,c);
                    tmp1*=matrix_double_get(r1,r,c);
                }
            }
            if(tmp0>tmp1){
                matrix_set(matrix_n,0,c,0);
            }else if(tmp0<tmp1){
                matrix_set(matrix_n,0,c,1);
            }else{
                //printf("\nError, the posterior_p is 0\n");
                matrix_set(matrix_n,0,c,-1);
            }
        }
        //check again
        flag=check_result(matrix_n,matrix_h);
        if(flag==1){
            return TIME-time;
        }else{
            //printf("\nflag=0\n");
        }
    }
    //matrix_printf(matrix_n);
    return -1;
}

int check_result(matrix *matrix_n,matrix* matrix_h){
    int r,c, flag=1;
    const int ldpc_n=matrix_n->col;
    const int ldpc_m=matrix_h->row;
    const int ldpc_k=ldpc_n-ldpc_m;
    for(c=0;c<ldpc_n;c++){
        if(matrix_get(matrix_n,0,c)==ERR){
            return 0;
        }
    }
    matrix * matrix_result=matrix_alloc(ldpc_m,1);
    matrix_mul(matrix_h,matrix_tran(matrix_n),matrix_result);
    for(r=0;r<ldpc_m;r++){
        if(matrix_get(matrix_result,r,0)%2!=0){
            flag=0;
            break;
        }
    }
    return flag;
}



int cl_decode(matrix *matrix_n,matrix* matrix_h){
    int flag;
    const int ldpc_n=matrix_n->col;
    const int ldpc_m=matrix_h->row;
    const int ldpc_k=ldpc_n-ldpc_m;
    int time=TIME;
	cl_event k_events[5];

    flag=check_result(matrix_n,matrix_h);
    if(flag==1)
        return 0;

    cl_device_id device_id;
    cl_context context;
    cl_command_queue commands;
    cl_program program;
    cl_kernel kernel[4];
    cl_uint nd;
    cl_int err;
    cl_mem r0,r1,q0,q1,prior_p0,prior_p1,cl_matrix_n,cl_matrix_h,cl_flag;

    context = create_context(CL_DEVICE_TYPE_GPU);
    commands = create_command_queue(context, &device_id);
    r0 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * ldpc_n*ldpc_m, NULL,NULL);
    r1 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * ldpc_n*ldpc_m, NULL,NULL);
    q0 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * ldpc_n*ldpc_m, NULL,NULL);
    q1 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * ldpc_n*ldpc_m, NULL,NULL);
    prior_p0 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * ldpc_n, NULL,NULL);
    prior_p1 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * ldpc_n, NULL,NULL);
    cl_matrix_n = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(char) * ldpc_n, NULL,NULL);
    cl_matrix_h = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(char) * ldpc_n* ldpc_m, NULL,NULL);
    cl_flag = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL,NULL);

    program=create_program(context,device_id,"./ieee_ldpc_decode_cl.c");

    kernel[0] = clCreateKernel(program, "ieee_ldpc_decode_init", &err);
    if(err!=0) printf("err int create kernel !!!!");

    err |= clSetKernelArg(kernel[0], 0, sizeof(int), &ldpc_n);
    err |= clSetKernelArg(kernel[0], 1, sizeof(int), &ldpc_m);
    err |= clSetKernelArg(kernel[0], 2, sizeof(cl_mem), &r0);
    err |= clSetKernelArg(kernel[0], 3, sizeof(cl_mem), &r1);
    err |= clSetKernelArg(kernel[0], 4, sizeof(cl_mem), &q0);
    err |= clSetKernelArg(kernel[0], 5, sizeof(cl_mem), &q1);
    err |= clSetKernelArg(kernel[0], 6, sizeof(cl_mem), &prior_p0);
    err |= clSetKernelArg(kernel[0], 7, sizeof(cl_mem), &prior_p1);
    err |= clSetKernelArg(kernel[0], 8, sizeof(cl_mem), &cl_matrix_n);
    err |= clSetKernelArg(kernel[0], 9, sizeof(cl_mem), &cl_matrix_h);


    if(err!=0) printf("err is not 0!!!!");
    err |= clEnqueueWriteBuffer(commands, cl_matrix_n, CL_TRUE, 0, sizeof(char ) * ldpc_n,matrix_to_array(matrix_n),0,NULL,NULL);
    err |= clEnqueueWriteBuffer(commands, cl_matrix_h, CL_TRUE, 0, sizeof(char ) * ldpc_n* ldpc_m,matrix_to_array(matrix_h),0,NULL,NULL);

    nd=2;
    size_t global[nd];
    size_t local[nd];
    global[0]=(size_t)ldpc_m+1;
    global[1]=(size_t)ldpc_n;
	local[0]=8;
	local[1]=32;
    err |= clEnqueueNDRangeKernel(commands, kernel[0], nd, NULL, global, NULL, 0,NULL,&k_events[4]);
    kernel[1] = clCreateKernel(program, "ieee_ldpc_decode_iteration", &err);
    err |= clSetKernelArg(kernel[1], 0, sizeof(int), &ldpc_n);
    err |= clSetKernelArg(kernel[1], 1, sizeof(int), &ldpc_m);
    err |= clSetKernelArg(kernel[1], 2, sizeof(cl_mem), &r0);
    err |= clSetKernelArg(kernel[1], 3, sizeof(cl_mem), &r1);
    err |= clSetKernelArg(kernel[1], 4, sizeof(cl_mem), &q0);
    err |= clSetKernelArg(kernel[1], 5, sizeof(cl_mem), &q1);
    err |= clSetKernelArg(kernel[1], 6, sizeof(cl_mem), &prior_p0);
    err |= clSetKernelArg(kernel[1], 7, sizeof(cl_mem), &prior_p1);
    err |= clSetKernelArg(kernel[1], 8, sizeof(cl_mem), &cl_matrix_n);
    err |= clSetKernelArg(kernel[1], 9, sizeof(cl_mem), &cl_matrix_h);
    err |= clSetKernelArg(kernel[1], 10, sizeof(cl_mem), &cl_flag);

    kernel[2] = clCreateKernel(program, "ieee_ldpc_decode_iteration2", &err);
    err |= clSetKernelArg(kernel[2], 0, sizeof(int), &ldpc_n);
    err |= clSetKernelArg(kernel[2], 1, sizeof(int), &ldpc_m);
    err |= clSetKernelArg(kernel[2], 2, sizeof(cl_mem), &r0);
    err |= clSetKernelArg(kernel[2], 3, sizeof(cl_mem), &r1);
    err |= clSetKernelArg(kernel[2], 4, sizeof(cl_mem), &q0);
    err |= clSetKernelArg(kernel[2], 5, sizeof(cl_mem), &q1);
    err |= clSetKernelArg(kernel[2], 6, sizeof(cl_mem), &prior_p0);
    err |= clSetKernelArg(kernel[2], 7, sizeof(cl_mem), &prior_p1);
    err |= clSetKernelArg(kernel[2], 8, sizeof(cl_mem), &cl_matrix_n);
    err |= clSetKernelArg(kernel[2], 9, sizeof(cl_mem), &cl_matrix_h);
    err |= clSetKernelArg(kernel[2], 10, sizeof(cl_mem), &cl_flag);

    kernel[3] = clCreateKernel(program, "ieee_ldpc_decode_iteration3", &err);
    err |= clSetKernelArg(kernel[3], 0, sizeof(int), &ldpc_n);
    err |= clSetKernelArg(kernel[3], 1, sizeof(int), &ldpc_m);
    err |= clSetKernelArg(kernel[3], 2, sizeof(cl_mem), &r0);
    err |= clSetKernelArg(kernel[3], 3, sizeof(cl_mem), &r1);
    err |= clSetKernelArg(kernel[3], 4, sizeof(cl_mem), &q0);
    err |= clSetKernelArg(kernel[3], 5, sizeof(cl_mem), &q1);
    err |= clSetKernelArg(kernel[3], 6, sizeof(cl_mem), &prior_p0);
    err |= clSetKernelArg(kernel[3], 7, sizeof(cl_mem), &prior_p1);
    err |= clSetKernelArg(kernel[3], 8, sizeof(cl_mem), &cl_matrix_n);
    err |= clSetKernelArg(kernel[3], 9, sizeof(cl_mem), &cl_matrix_h);
    err |= clSetKernelArg(kernel[3], 10, sizeof(cl_mem), &cl_flag);
    global[0]=(size_t)ldpc_m;
    int* p_flag=(int*)malloc(sizeof(int));
    *p_flag=0;
    time=TIME;
    char * matrix_n_array=(char*)malloc(sizeof(char)*ldpc_n);
    matrix * matrix_n_result;


	//******************
	while(*p_flag!=1 && time!=0){
		err = clEnqueueNDRangeKernel(commands, kernel[1], nd, NULL, global, local, 1,&k_events[4],&k_events[0]);

		err = clEnqueueNDRangeKernel(commands, kernel[2], nd, NULL, global, local, 1,&k_events[0],&k_events[1]);

		err = clEnqueueNDRangeKernel(commands, kernel[3], nd, NULL, global, local, 1,&k_events[1],&k_events[2]);

		err |= clEnqueueReadBuffer(commands, cl_flag, CL_TRUE, 0, sizeof(int) ,p_flag, 1, &k_events[2], &k_events[3]);

		time--;
	}
	//printf("Iteration is finished\n");
	//*****************
	clFinish(commands);
	err |= clEnqueueReadBuffer(commands, cl_matrix_n, CL_TRUE, 0, sizeof(char ) * ldpc_n,matrix_n_array, 0, NULL, NULL);
	matrix_n_result=matrix_from_array(matrix_n_array,1,ldpc_n);
	matrix_copy(matrix_n_result,matrix_n);

	if(err!=0) printf("1err is not 0!!!!\n");
	//printf("Before release\n");

	clReleaseProgram(program);
	clReleaseKernel(kernel[3]);
	clReleaseKernel(kernel[2]);
	clReleaseKernel(kernel[1]);
	clReleaseKernel(kernel[0]);
	//printf("Release kernel finished\n");
	clReleaseMemObject(r0);
	clReleaseMemObject(r1);
	clReleaseMemObject(q0);
	clReleaseMemObject(q1);
	clReleaseMemObject(prior_p0);
	clReleaseMemObject(prior_p1);
	clReleaseMemObject(cl_matrix_n);
	clReleaseMemObject(cl_matrix_h);
	clReleaseMemObject(cl_flag);
	//printf("Release MemObject finished\n");
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
	if(time==0)
		return -1;
	else 
		return TIME-time;
}

