#include "csapp.h"

void echo(int connfd);

int main(int argc, char **argv)
{
	int listenfd,connfd,port,clientlen;
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char *haddrp;
	if(argc != 2)
	{
		fprintf(stderr,"usage: %s <port>\n",argv[0]);
		exit(0);
	}
	port = atoi(argv[1]);

	listenfd = Open_listenfd(port);								//返回一个监听套接字,等待客户端的连接请求
	while(1)
	{
		clientlen = sizeof(clientaddr);
		//返回一个已连接描述符,可以直接进行I/O操作
		connfd = Accept(listenfd,(SA *)&clientaddr,&clientlen);

		//determin the domain name and ip address of the client 
		hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,sizeof(clientaddr.sin_addr.s_addr),AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);
		printf("server connected to %s (%s)\n",hp->h_name,haddrp);

		echo(connfd);
		Close(connfd);
		//printf("client Close.\n");
	}
	exit(0);
}
