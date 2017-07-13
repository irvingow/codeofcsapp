#include "csapp.h"

//used to handler when a child process ends
void handler(int sig)
{
	pid_t pid;
	while((pid = waitpid(-1,NULL,0)) > 0)
		printf("reaped child process %d\n",(int)pid);
	if(errno != ECHILD)
		unix_error("waitpid errno");
	Sleep(2);
	printf("I was called\n");
	return;
}

int main()
{
	int i,n;
	char buf[MAXBUF];
	pid_t pid;

	if(signal(SIGCHLD,handler) == SIG_ERR)
		unix_error("signal error");

	for(int i = 0;i < 3;i++)
	{
		if((pid = Fork()) == 0)
		{
			printf("child process %d\n",(int)getpid());
			Sleep(1);
			exit(0);
		}
	}

	while((n = read(STDIN_FILENO,buf,sizeof(buf))) < 0)
		if(errno != EINTR)
			unix_error("read error");

	printf("father process input\n");
	return 0;
}
