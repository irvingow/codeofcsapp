#include "csapp.h"

void echo(int connfd)
{
	size_t n;
	char buf[MAXLINE];
	rio_t rio;

	Rio_readinitb(&rio,connfd);
	//当我们结束客户端进程后,n就返回0,从循环跳出
	while((n = Rio_readlineb(&rio,buf,MAXLINE)) != 0)
	{
		printf("server received %d bytes\n",n);
		Rio_writen(connfd,buf,n);
	}
}
