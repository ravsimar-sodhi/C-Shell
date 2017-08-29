#include  <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include  <sys/types.h>
#include  <sys/wait.h>
int main()
{
	pid_t pid;
	pid=fork();
	char * s[10];
	char * comm=malloc(100*sizeof(char));
	if(pid==0)
	{
		scanf("%s",comm);
		s[0]=comm;
		s[1]=NULL;
		execvp(comm,s);
		printf("FAILED");
	}
	else if(pid<0)
	{
		printf("FORK FAILED\n");
	}
	else
	{
		int status;
		wait(NULL);
		printf("ENDED");
	}
	return 0;
}