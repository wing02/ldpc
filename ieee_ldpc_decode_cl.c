
__kernel void ieee_ldpc_decode_init(const int ldpc_n, const int ldpc_m, __global float * r0, __global float *r1, __global float *q0, __global float * q1, __global float * prior_p0, __global float * prior_p1,__global char * matrix_n,__global char * matrix_h){
	size_t r=get_global_id(0);//ldpc_m
	size_t c=get_global_id(1);//ldpc_n
	if(matrix_n[c]==0){
		if(r<ldpc_m){
			r0[r*ldpc_n+c]=0;
			r1[r*ldpc_n+c]=0;
			q0[r*ldpc_n+c]=1.0;
			q1[r*ldpc_n+c]=0;
		}else if(r==ldpc_m){
			prior_p0[c]=1.0;
			prior_p1[c]=0;
		}
	}else if(matrix_n[c]==1){
		if(r<ldpc_m){
			r0[r*ldpc_n+c]=0;
			r1[r*ldpc_n+c]=0;
			q0[r*ldpc_n+c]=0;
			q1[r*ldpc_n+c]=1.0;
		}else if(r==ldpc_m){
			prior_p0[c]=0;
			prior_p1[c]=1.0;
		}
	}else if(matrix_n[c]==-1){
		if(r<ldpc_m){
			r0[r*ldpc_n+c]=0;
			r1[r*ldpc_n+c]=0;
			q0[r*ldpc_n+c]=0.5;
			q1[r*ldpc_n+c]=0.5;
		}else if(r==ldpc_m){
			prior_p0[c]=0.5;
			prior_p1[c]=0.5;
		}
	}
}


__kernel void ieee_ldpc_decode_iteration(const int ldpc_n, const int ldpc_m , __global float * r0, __global float *r1, __global float *q0, __global float * q1, __global float * prior_p0, __global float * prior_p1,__global char * matrix_n,__global char * matrix_h,__global int *flag){
	int r=get_global_id(0);//ldpc_m
	int c=get_global_id(1);//ldpc_n
	int cc;
	float d_tmp;
	if(matrix_h[r*ldpc_n+c]==1){
		d_tmp=1.0;
		for(cc=0;cc<ldpc_n;cc++){
			if(cc!=c && matrix_h[r*ldpc_n+cc]==1){
				d_tmp*=q0[r*ldpc_n+cc]-q1[r*ldpc_n+cc];
			}
		}
		r0[r*ldpc_n+c]=(1+d_tmp)/2;
		r1[r*ldpc_n+c]=(1-d_tmp)/2;
	}
	if(r==0 &&c==0)
		*flag=1;
}


__kernel void ieee_ldpc_decode_iteration2(const int ldpc_n, const int ldpc_m , __global float * r0, __global float *r1, __global float *q0, __global float * q1, __global float * prior_p0, __global float * prior_p1,__global char * matrix_n,__global char * matrix_h,__global int *flag){
	int r=get_global_id(0);//ldpc_m
	int c=get_global_id(1);//ldpc_n
	int cc,rr;
	float d_tmp,tmp0,tmp1;
	// refresh q0 q1
	if(matrix_h[r*ldpc_n+c]==1){
		tmp0=prior_p0[c];
		tmp1=prior_p1[c];
		for(rr=0;rr<ldpc_m;rr++){
			if(rr!=r && matrix_h[rr*ldpc_n+c]==1){
				tmp0*=r0[rr*ldpc_n+c];
				tmp1*=r1[rr*ldpc_n+c];
			}
		}
		d_tmp=tmp0+tmp1;
		tmp0=tmp0/d_tmp;
		tmp1=tmp1/d_tmp;
		q0[r*ldpc_n+c]=tmp0;
		q1[r*ldpc_n+c]=tmp1;
	}
	// refresh prior_p0 prior_p1 from r0 r1
	if(r==0){
		tmp0=prior_p0[c];
		tmp1=prior_p1[c];
		for(rr=0;rr<ldpc_m;rr++){
			if(matrix_h[rr*ldpc_n+c]==1){
				tmp0*=r0[rr*ldpc_n+c];
				tmp1*=r1[rr*ldpc_n+c];
			}
		}
		if(tmp0>tmp1){
			matrix_n[c]=0;
		}else if(tmp0<tmp1){
			matrix_n[c]=1;
		}else if(tmp0==tmp1){
			matrix_n[c]=-1;
		}

	} 
	if(r==0 &&c==0)
		*flag=1;
}

__kernel void ieee_ldpc_decode_iteration3(const int ldpc_n, const int ldpc_m , __global float * r0, __global float *r1, __global float *q0, __global float * q1, __global float * prior_p0, __global float * prior_p1,__global char * matrix_n,__global char * matrix_h,__global int *flag){
	int r=get_global_id(0);//ldpc_m
	int c=get_global_id(1);//ldpc_n
	int cc,rr;
	int tmp;

	if(r==0){
		if(matrix_n[c]==-1){
			*flag=0;
		}
	}
	if(c==0){
		int k;
		tmp=0;
		for(k=0;k<ldpc_n;k++){
			if(	matrix_h[r*ldpc_n+k]*matrix_n[k]==1){
				tmp++;
			}
		}
		//matrix_zero[r]=tmp;
		if(tmp%2!=0){
			*flag=0;
		}
	}
}
