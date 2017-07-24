#include "csapp.h"

int main(int argc, char **argv)
{
	struct stat stat;
	char *type,*readok;

	Stat(argv[1],&stat);
	if(S_ISREG(stat.st_mode))		//determing the file type 
		type = "regular";
	else if(S_ISDIR(stat.st_mode))
		type = "directory";
	else 
		type = "other";
	
	//S_IRUSR read permission, owner
	if((stat.st_mode && S_IRUSR))	//check read access
		readok = "yes";
	else 
		readok = "false";

	printf("type: %s, read: %s\n",type,readok);
	exit(0);
}
