#include "csapp.h"

jmp_buf buf;

int error1 = 0;
int error2 = 1;

void foo(void),bar(void);

int main()
{
	int rc;

	rc = setjmp(buf);
	printf("may be i will appear two time\n");
	if(rc == 0)
		foo();
	else if(rc == 1)
		printf("Detect an error1 condition in foo\n");
	else if(rc == 2)
		printf("Detect an error2 condition in foo\n");
	else
		printf("Unknown error condition in foo\n");
	exit(0);
}

void foo(void)
{
	if(error1)
		longjmp(buf,1);
	bar();
}

void bar(void)
{
	if(error2)
		longjmp(buf,2);
}
