#include "csapp.h"

pid_t pid;
static int counter = 2;

void handler(int sig)
{
	counter--;
	printf("%d",counter);
	fflush(stdout);
	exit(0);
}

void handler1(int sig)
{
	counter++;
	printf("%d",counter);
	fflush(stdout);
	exit(0);

}

int main()
{
	signal(SIGUSR1,handler1);

	printf("%d",counter);
	fflush(stdout);

	if((pid = Fork()) == 0)
	{
		signal(SIGUSR1,handler1);
		while(1){};
	}
	kill(pid,SIGUSR1);
	waitpid(-1,NULL,0);
	counter += 1;
	printf("%d",counter);
	exit(0);
}
