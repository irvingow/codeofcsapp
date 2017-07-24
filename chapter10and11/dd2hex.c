#include "csapp.h"

int main(int argc, char **argv)
{
	struct in_addr inaddr;			//addr in network byte order(big endian)
	unsigned int addr;				//addr in host byte order(most small endian)

	if(argc != 2)
	{
		fprintf(stderr,"usage: %s <dotted-decimal>\n",argv[0]);
		exit(0);
	}
	//compared with hex2dd.c, we just use argv[1] as our input and do not use sscanf.
	if(inet_aton(argv[1],&inaddr) == 0)
	{
		app_error("inet_ntoa error");
	}
	addr = ntohl(inaddr.s_addr);
	printf("0x%x\n",addr);

	exit(0);
}

