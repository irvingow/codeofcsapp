#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
int main()
{
	pid_t pid;
	pid = fork();

	if(pid < 0)
	{
		printf("error in fork()\n");
	}
	else if(pid == 0)
	{
		printf("I am the child process, my id is:%d\n",getpid());
		return 0;
	}
	else
		printf("I am the parent process, my id is:%d\n",getpid());
	printf("my child process pid is:%d\n",pid);
	return 0;
}
