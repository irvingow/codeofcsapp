#include "csapp.h"
sem_t mutex;

char *ctime_ts(const time_t *timep, char *privatep)
{
	char *sharedp;

	P(&mutex);
	sharedp = ctime(timep);
	strcpy(privatep,sharedp);		//copy string from sharedp to privatep 
	V(&mutex);
	return privatep;
}
