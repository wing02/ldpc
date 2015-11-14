#define SIZE 1000
#define VAR char
__kernel void matrix_mul(const int Mdim, const int Ndim, const int Pdim, __global VAR *A, __global VAR *B, __global VAR *C)
{
	int k,j;
	int i = get_global_id(0);
	VAR tmp;
	VAR Awrk[SIZE];
	if (i<Ndim)
	{
		for(k=0;k<Pdim;k++)
			Awrk[k]=A[i*Pdim+k];
		for(j=0;j<Mdim;j++)
		{
			tmp = 0.0;
			for (k = 0; k<Pdim; k++)
				tmp += Awrk[k] * B[k*Mdim + j];
			C[i*Mdim + j] = tmp;
		}
	}
}


