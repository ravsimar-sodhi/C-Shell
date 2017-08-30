#include<unistd.h>
#include<stdio.h>
void cd(const char* path)
{
    if(chdir(path) == 0)
        return ;
    else
    {
        printf("Invalid path\n");
    }
    return ;
}
