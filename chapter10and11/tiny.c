#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

int main(int argc, char **argv)
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    //check command line args
    if(argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);
    while(1)
    {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        doit(connfd);
        Close(connfd);
    }
}

void doit(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    //read request line and headers
    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);
    if(strcasecmp(method, "GET"))
    {
        clienterror(fd, method, "501", "Not Implemented", "Tiny does not implement this method");
        return;
    }
    read_requesthdrs(&rio);

    //parse URI from GET request
    is_static = parse_uri(uri, filename, cgiargs);
    if(stat(filename, &sbuf) < 0)
    {
        clienterror(fd, method, "404", "Not found", "Tiny could not fild this file");
        return;
    }

    if(is_static)//serve static content
    {
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
        {
            clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't read this file");
            return;
        }
    }
    else
    {
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
        {
            clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't run the CGI program");
            return;
        }
        serve_dynamic(fd, filename, cgiargs);
    }
}


void clienterror(int fd, char *cause, char *errum, char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXLINE];

    //build the HTTP response body
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff""\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    //print the HTTP response
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, buf, strlen(body));
}


void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    while(strcmp(buf, "\r\n"))
    {
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}

int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;
    //we suppose that the executable files only exist in cgi-bin.
    if(!strstr(uri, "cgi-bin"))					//static content
    {
        //clear the cgiargs
        strcpy(cgiargs, "");
        //convert uri to a relative Unix pathname
        strcpy(filename, ".");
        strcpy(filename, uri);
        //if the URI ends with a '/' character, the we append the default name
        if(uri[strlen(uri) - 1] == '/')
            strcat(filename, "home.html");
        return 1;
    }
    else										//dynamic content
    {
        ptr = index(uri, '?');
        if(ptr)
        {
            strcpy(cgiargs, ptr + 1);
            *ptr = '\0';
        }
        else
        {
            strcpy(cgiargs, "");
        }
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}


void serve_static(int fd, char *filename, int filesize)
{
	int srcfd;
	char *srcp,filetype[MAXLINE],buf[MAXLINE];

	//send response headers to client 
	get_filetype(filename,filetype);
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	sprintf(buf, "%sServer: Tiny Web Server\r\n",buf);
    sprintf(buf, "%sContent-length: %d\r\n\r\n", buf,filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n",buf,filetype);
    Rio_writen(fd, buf, strlen(buf));

	//send response body to client
	srcfd = Open(filename,O_RDONLY,0);
	//the call to mmap maps the first filesize bytes of file 
	//srcfd to a private read-only area of virtual memory that 
	//starts at address srcp
	srcp = Mmap(0,filesize,PROT_READ,MAP_PRIVATE,srcfd,0);
	Close(srcfd);
    Rio_writen(fd, srcp, filesize);
	//free the mapped virtual memory area 
	Munmap(srcp,filesize);
}

/*
 * get_filetype - derive file type from file name
 * */
void get_filetype(char *filename,char *filetype)
{
	if(strstr(filename,".html"))
		strcpy(filetype,"text/html");
	else if(strstr(filetype,".gif"))
		strcpy(filetype,"image/gif");
	else if(strstr(filetype,".jpg"))
		strcpy(filetype,"image/jpg");
	else 
		strcpy(filetype,"text/plain");
}


void serve_dynamic(int fd, char *filename, char *cgiargs)
{
	char buf[MAXLINE], *emptylist[] = {NULL};

	/* return first part of HTTP response*/
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Server: Tiny Web Server\r\n");
	Rio_writen(fd, buf, strlen(buf));

	if(Fork() == 0)//child
	{
		//real server would set all CGI vars here.
		setenv("QUERY_STRING",cgiargs,1);
		Dup2(fd,STDOUT_FILENO);							//redirect stdout to client 
		Execve(filename,emptylist,environ);				//run CGI program
	}
	Wait(NULL);//Parent waits for and reaps child
}
