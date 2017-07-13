#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

//sleep for a random period between [0,MAX_SLEEP] us.
#define MAX_SLEEP 100000

//Macro that maps val into the range [0,RAND_MAX]
#define CONVERT(val) (((double)val)/(double)RAND_MAX)

pid_t Fork(void)
{
	static struct timeval time;
	unsigned bool,secs;
	pid_t pid;

	//generate a different seed each time the function is called
	gettimeofday(&time,NULL);
	srand(time.tv_usec);
	
	//determin which to sleep and for how long
	bool = (unsigned) (CONVERT(rand()) + 0.5);
	secs = (unsigned) (CONVERT(rand()) * MAX_SLEEP);

	//call the real fork function.
	if((pid = fork()) < 0)
	{
		return pid;
	}

	//randomly decide to sleep in the parent or the child
	if(pid == 0)//child
	{
		if(bool)//only one of the child and the parent will sleep
		{
			usleep(secs);
		}
	}
	else{
		if(!bool)
		{
			usleep(secs);
		}
	}

	//return the PID like a normal fork call.
	return pid;
}
