#include "csapp.h"

int main(int argc, char **argv)
{
	struct in_addr inaddr;			//addr in network byte order(big endian)
	unsigned int addr;				//addr in host byte order(most small endian)

	if(argc != 2)
	{
		fprintf(stderr,"usage: %s <hex number>\n",argv[0]);
		exit(0);
	}
	//we do not use argv[1] for it's not easy for us to convert char* to hex number.
	//sscanf reads formatted input from a string.
	sscanf(argv[1],"%x",&addr);		//we get the input from stdin, and store it in addr.
	//htonl()将32位整数从主机字节序转换为网络字节序
	inaddr.s_addr = htonl(addr);
	printf("%s\n",inet_ntoa(inaddr));

	exit(0);
}
