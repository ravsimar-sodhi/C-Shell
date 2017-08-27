#include "ls.c"
#include "pwd.c"
#include "cd.c"
#include "user@host.c"

#include<string.h>
char* getInput()
{
    ssize_t read;
    char* buffer = NULL;
    size_t bufsize = 0;
    while(1)
    {
        read = getline(&buffer,&bufsize,stdin);
        if(read != -1)
        {
            buffer[read-1] = '\0';
            return buffer;
        }
        else
            break;
    }
}
void parseInput(char* line)
{
    char* token = strtok(line,";");
    while(token != NULL)
    {
        printf("%s\n",token);
        token = strtok(NULL,";");
    }
    return ;
}

int main()
{
    const char* HOME = getPWD();
    //printf("%s",HOME);    
    do
    {
        char* fullPWD = getPWD();
        char* PWD = malloc(sizeof(char) * strlen(fullPWD));
        //printf("%s",fullPWD);
        if(strstr(fullPWD,HOME) != NULL)
        {
            PWD[0] = '~';
            PWD[1] = '\0';
            //printf("substriing");
            strcat(PWD,fullPWD+strlen(HOME));
        }  
        else
            PWD = fullPWD;      
        printf("<%s@%s:%s>",getUserName(),getHostName(),PWD);
        char* line = getInput();
        parseInput(line);
        cd("/home");
        //printf("%s",line);
    }
    while(1);
    return 0;
}
