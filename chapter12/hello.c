#include "csapp.h"

void *thread(void *vargp);

int main()
{
	pthread_t tid;
	Pthread_create(&tid,NULL,thread,NULL);
	//explictly waits tid terminating and reaps
	//any memory resources held by the terminated thread
	Pthread_join(tid,NULL);
	exit(0);
}

void *thread(void *vargp)//thread routine
{
	printf("Hello, world\n");
	return NULL;
}
