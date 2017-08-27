#include<unistd.h>
#include<stdio.h>
void cd(const char* path)
{
    if(!chdir(path))
    return ;
}
