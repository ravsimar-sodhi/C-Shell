#include<unistd.h>
#include<stdio.h>
char* getPWD()
{
    char* buf = NULL;
    buf = getcwd(buf,100);
    if (buf == NULL)
        // print: Error obtaining current directory//
    return buf;
}
int main()
{
    printf("%s\n",getPWD());
    return 0;
}
