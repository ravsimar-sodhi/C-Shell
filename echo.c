#include<stdio.h>
#include<stdlib.h>

char* echo(char* arg)
{
    int i;
    int size = 0;
    while(arg[size] != '\0') size++;
    size--;
    if((arg[0] == '"' && arg[size] != '"') || (arg[size] == '"' && arg[0] != '"'))
        printf("Error");
    if((arg[0] == '\'' && arg[size] != '\'') || (arg[size] == '\'' && arg[0] != '\''))
        printf("Error");
    if(arg[0] != '"' && arg[0] != '\'') return arg;
    char* str = malloc(sizeof(char) * 1000);
    int k = 0;
    
    char rem = ' ';
    for(i=0;i<size;i++)
    {
        if(rem == ' ' && (arg[i] == '"' || arg[i] == '\''))
            rem = arg[i];
        if(arg[i] != arg[0] && arg[i] != rem)
            str[k++] = arg[i];
    }
    str[k] = '\0';
    return str;
}
