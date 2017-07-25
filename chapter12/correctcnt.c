#include "csapp.h"

void *thread(void *vargp);//thread routine prototype

//global shared variable
volatile int cnt = 0;//counter
sem_t mutex;		 //semaphore that protects counter

int main(int argc, char **argv)
{
    int niters;
    pthread_t tid1, tid2;

    //check input argument
    if(argc != 2)
    {
        fprintf(stderr, "usage: %s <niters>\n", argv[0]);
        exit(0);
    }
    niters = atoi(argv[1]);

    Sem_init(&mutex, 0, 1); //initilize mutex = 1
    //create threads and wait for them to finish
    Pthread_create(&tid1, NULL, thread, &niters);
    Pthread_create(&tid2, NULL, thread, &niters);
    Pthread_join(tid1, NULL);
    Pthread_join(tid2, NULL);

    //check result
    if(cnt != (2 * niters))
        printf("BOOM! cnt=%d\n", cnt);
    else
        printf("OK cnt=%d\n", cnt);
    exit(0);
}

//thread routine
void *thread(void *vargp)
{
    int i, niters = *((int *)vargp);

    for(i = 0; i < niters; ++i)
    {
        P(&mutex);		//locking the mutex and mutex will be zero so
        //when the other thread invokes P(&mutex), it will be waiting
        //until mutex become non-zero.
        cnt++;
        V(&mutex);		//unlocking the mutex and mutex will be non-zero
        //so threads waiting for mutex becoming non-zero can complete its
        //P operation by decreasing s
    }

    return NULL;
}
