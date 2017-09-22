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
    for(int i=0;i<1000000;i++);
    dup2(a,1);
    fprintf(stdout,"SS");
    return 0;
}
