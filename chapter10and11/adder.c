#include "csapp.h"

int main(void)
{
	char *buf, *p;
	char arg1[MAXLINE],arg2[MAXLINE],content[MAXLINE];
	int n1 = 0, n2 = 0;

	//extract the two arguments
	//char *getenv(const char* name) searches for the environment string 
	//pointed to by name and returns the associated value to the string 
	if((buf = getenv("QUERY_STRING")) != NULL)
	{
		p = strchr(buf,'&');
		*p = '\0';
		strcpy(arg1,buf);
		strcpy(arg2,p+1);
		n1 = atoi(arg1);
		n2 = atoi(arg2);
	}

	//make the response body
	//int sprintf(char *str, const char *format, ...) sends formatted 
	//output to a string pointed to by str.
	sprintf(content,"Welcome to add.com: ");
	//in fact, content is the pointer to the first element of the array
	//so we need to add the previous content to avoid overwriting. That's
	//why we need ( "%s",content ).
	sprintf(content,"%sTHE Internet addition portal.\r\n<p>",content);
	sprintf(content,"%sThe answer is %d + %d = %d\r\n<p>",content,n1,n2,n1+n2);
	sprintf(content,"%sThanks for visiting!\r\n",content);

	//generate the HTTP response
	printf("Content-length: %d\r\n",(int)strlen(content));
	printf("Content-type: text/html\r\n\r\n");
	printf("%s",content);
	fflush(stdout);
	exit(0);
}
