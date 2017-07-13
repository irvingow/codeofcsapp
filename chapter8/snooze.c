#include "csapp.h"

void handler(int sig)
{
	return;
}

unsigned int snooze(unsigned int secs)
{
	unsigned int lefttime;
	lefttime = sleep(secs);
	
	printf("Slept for %d of %d sec\n",(secs-lefttime),secs);
	return lefttime;
}

int main(int argc, char **argv)
{
	char *num = argv[1];
	int secs = atoi(num);
	if(signal(SIGINT, handler) == SIG_ERR)
		unix_error("signal error");
	snooze(secs);
	return 0;
}
