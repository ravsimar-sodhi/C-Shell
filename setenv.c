#include<stdlib.h>
#include<stdio.h>
int main()
{
    setenv("test","1",1);
    setenv("test","2",0);
    char* res = getenv("test");
    printf("%s",res);
}