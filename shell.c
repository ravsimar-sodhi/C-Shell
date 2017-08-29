#include "ls.c"
#include "pwd.c"
#include "cd.c"
#include "user@host.c"
#include "pinfo.c"

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

int parseInput(char* line,char*** commQ)
{
    int i = 0 ;
    (*commQ)[i] = strtok(line,";");
    while(commQ[i] != NULL)
    {
        printf("%s\n",(*commQ)[i]);
        i++;
        (*commQ)[i] = strtok(NULL,";");
    }
    printf("parse end\n");
    return i-1;
}

int main()
{
    const char* HOME = getPWD();
    int i;
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
        char** commQ =  malloc(sizeof(char*) * 100);
        int commN = 0;
        commN = parseInput(line,&commQ);
        for (i=0;i<commN;i++)
        {
            printf("%s\n",commQ[i]);
        }
        free(PWD);
        free(commQ);
        //ls(".");
       // printf("%s\n",pinfo(getpid()));
//      printf("%s\n",pinfo(12815));
        //cd("/home");
        //printf("%s",line);
    }
    while(1);
    return 0;
}
