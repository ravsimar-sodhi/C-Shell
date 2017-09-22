#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
    int fd1 = creat("output.txt", 0644) ;
    dup2(fd1, STDOUT_FILENO);
    printf("works!");
    close(fd1);
    return 0;
}