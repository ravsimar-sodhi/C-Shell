#include<unistd.h>
#include<stdio.h>
char* getPWD()
{
    char* buf=NULL;
    buf = getcwd(buf,100);
    if (buf == NULL)
        // print: Error obtaining current directory//
    return buf;
}
