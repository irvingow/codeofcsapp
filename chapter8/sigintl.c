#include "csapp.h"
typedef void (*sighandler_t)(int);
void handler(int sig)
{
	printf("Caught SIGINT\n");
	exit(0);
}

int main()
{
	sighandler_t signalhandler,handler1;
	if((signalhandler = signal(SIGINT,handler)) == SIG_ERR)
		unix_error("signal error");
	handler1 = signal(SIGINT,signalhandler);
	signal(SIGINT,handler1);
	pause();
	exit(0);
}
