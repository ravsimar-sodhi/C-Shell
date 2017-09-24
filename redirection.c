#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
    int fd1 = creat("output.txt", 0644);
    int a=dup(1);
    dup2(fd1, STDOUT_FILENO);
    fprintf(stdout,"works!");
    close(fd1);
    dup2(a,1);
    printf("SS");
    return 0;
}
