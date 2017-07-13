#include "csapp.h"

int main()
{
	pid_t pid;
	int x= 1;

	pid = Fork();
	if(pid == 0)
	{
		printf("child : x=%d/n",++x);
		exit(0);
	}
	if(pid == 0)
		printf("hello, world!");
	printf("parent : x=%d/n",--x);
	exit(0);
}
