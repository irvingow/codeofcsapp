#include "csapp.h"
void echo(int connfd);

void sigchld_handler(int sig)
{
	//WNOHANG:
	//  The waitpid() function shall not suspend execution of the calling
	//thread if status is not immediately available for one of the 
	//child processes specified by pid.
	while(waitpid(-1,0,WNOHANG) > 0)//如果等待集合中任何子进程都还没终止,立刻返回
		;
	return;
}

int main(int argc, char **argv)
{
	int listenfd, connfd, port;
	socklen_t clientlen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;

	if(argc != 2)
	{
		fprintf(stderr,"usage: %s <port>\n",argv[0]);
		exit(0);
	}
	port = atoi(argv[1]);

	Signal(SIGCHLD,sigchld_handler);
	listenfd = Open_listenfd(port);
	while(1)
	{
		connfd = Accept(listenfd,(SA *)&clientaddr, &clientlen);
		if(Fork() == 0)
		{
			Close(listenfd);	//child closes its listening socket 
			echo(connfd);		//child services client 
			Close(connfd);	//child closes connection with client 
			exit(0);
		}
		Close(connfd);//parent closes connected socket (important!)
	}
	exit(0);
}
