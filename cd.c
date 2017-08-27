#include<unistd.h>
#include<stdio.h>
void cd(const char* path)
{
    if(!chdir(path))
        //print: Error changing directory//
    printf("fdaf");
    return ;
}
