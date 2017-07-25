#include "csapp.h"
#define MAXTHREADS 32

void *sum(void *vargp);

//Global shared variables
long psum[MAXTHREADS];			//Partial sum computed by each thread 
long nelems_per_thread;			//Number of elements summed by each thread 

int main(int argc, char **argv)
{
	long i,nelems,log_nelems, nthreads, result = 0;
	pthread_t tid[MAXTHREADS];
	int myid[MAXTHREADS];

	//Get input arguments 
	if(argc != 3)
	{
        fprintf(stderr, "usage: %s <nthreads> <log_nelems>\n", argv[0]);
        exit(0);
	}
	nthreads = atoi(argv[1]);
	log_nelems = atoi(argv[2]);
	nelems = (1L << log_nelems);
	nelems_per_thread = nelems / nthreads;

	//Create peer threads and wait for them to finish
	for(i = 0; i < nthreads; ++i)
	{
		myid[i] = i;
		Pthread_create(&tid[i],NULL,sum,&myid[i]);
	}
	for(i = 0; i < nthreads; i++)
	{
		Pthread_join(tid[i],NULL);
	}

	//Add up the Partial sums computed by each thread 
	for(i = 0; i < nthreads; ++i)
		result += psum[i];

	//Check final answer
	if(result != (nelems * (nelems-1)) / 2)
		printf("Error: result = %ld\n",result);

	exit(0);
}

void *sum(void *vargp)
{
	int myid = *((int *)vargp);				//Extract the thread ID
	long start = myid * nelems_per_thread;	//Start element index
	long end = start + nelems_per_thread;	//End element index 
	long i, sum = 0;

	for(i = start; i < end; i++)
		sum += i;
	psum[myid] = sum;

	return NULL;
}
