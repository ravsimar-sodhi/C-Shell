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
  //  char** tokenHolder = malloc(sizeof(char*) * );
    while(token != NULL)
    {
        printf("%s\n",token);
        token = strtok(NULL,";");
    }
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
        printf("\033[1;32m<%s@\033[0m\033[1;32m%s\033[0m:\033[1;34m%s>\033[0m",getUserName(),getHostName(),PWD);
        
        char* line = getInput();
        parseInput(line);
        ls(".");
        //cd("/home");
        //printf("%s",line);
    }
    while(1);
    return 0;
}
