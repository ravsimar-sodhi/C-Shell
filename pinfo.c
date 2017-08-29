#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
	char* spath=malloc(100),ppath[50],expath[100010],*status=malloc(5),*mem=malloc(100);
	char buf[100010];
	int pid=2520;
	sprintf(spath,"/proc/%d/stat",pid);
	sprintf(ppath,"/proc/%d/cmdline",pid);
	int fd = open(ppath,O_RDONLY);
	read(fd,expath,100010);
	fd = open(spath,O_RDONLY);
	read(fd,buf,100010);
	char * token = strtok(buf," ");
	int i=1;
	while(token!=NULL)
	{
		i++;
		token = strtok(NULL," ");
		if(i==3)
			status=token;
		if(i==23)
			mem=token;
	}
	printf("pid              -  %d\nProcess Status   -   %s\nMemory           -   %s{virtual memory\nExecutable Path  -  %s\n",pid,status,mem,expath);
	return 0;
}