#define SIZE 1000
__kernel void matrix_mul(const int Mdim, const int Ndim, const int Pdim, __global double *A, __global double *B, __global double *C,__local double * Bwrk)
{
	int k,j;
	int i = get_global_id(0);
	int iloc=get_local_id(0);
	int nloc=get_local_size(0);
	double tmp;
	double Awrk[SIZE];
	if (i<Ndim)
	{
		for(k=0;k<Pdim;k++)
			Awrk[k]=A[i*Pdim+k];
		for(j=0;j<Mdim;j++)
		{
			for (k=iloc;k<Pdim;k=k+nloc)
				Bwrk[k]=B[k*Mdim+j];
			barrier(CLK_LOCAL_MEM_FENCE);
			tmp = 0;
			for (k = 0; k<Pdim; k++)
				tmp += Awrk[k] * Bwrk[k];
			C[i*Mdim + j] = tmp;
		}
	}
}


