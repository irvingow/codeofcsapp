#include "csapp.h"

int main(int argc, char **argv)
{
    int clientfd, port;
    char *host, buf[MAXLINE];
    rio_t rio;

    if(argc != 3)
    {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);

    clientfd = Open_clientfd(host, port);		//返回一个已经连接到服务器的套接字,可以进行I/O读写
    Rio_readinitb(&rio, clientfd);				//初始化

    while(Fgets(buf, MAXLINE, stdin) != NULL)		//fgets reads a line from stdin,and stores it into
        //the string pointed to by buf, MAXLINE is the max
        //number of characters to be read(including the
        //final null-character)
    {
        Rio_writen(clientfd, buf, strlen(buf));	//通过套接字向服务器发送数据
        Rio_readlineb(&rio, buf, MAXLINE);		//通过套接字读取服务器发来的数据
        Fputs(buf, stdout);						//fputs writes a string to stdout but not including
        //the null character
    }
    Close(clientfd);
    exit(0);
}
