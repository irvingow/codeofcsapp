void multvec(int *x,int *y,int *z,int n)
{
	int i;
	for(i = 0;i<n;++i)
		z[i] = x[i]*y[i];
	swap(z[0],z[1]);
}
