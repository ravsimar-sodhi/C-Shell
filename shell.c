#include "ls.c"
#include "pwd.c"
#include "cd.c"
#include "user@host.c"
#include "pinfo.c"

#include<string.h>

char* HOME;
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
    while((*commQ)[i] != NULL)
    {
        (*commQ)[++i] = strtok(NULL,";");
    }
    return i;
}
int parseCommand(char* fullComm,char** mainComm, char*** args)
{
    int i = 0;
    char* token = strtok(fullComm," ");
    *mainComm = token;
    while(token != NULL)
    {
        token = strtok(NULL," ");
        (*args)[i++] = token;
    }
    return i-1;
}
void checkBuiltIn(char* comm,char** args)
{
   // if(comm == "ls")
    char* output;
    if(strcmp(comm,"pwd") == 0)
    {
        output = getPWD();
        printf("%s\n",output);
    }
    else if(strcmp(comm,"cd") == 0)
    {
        if(args[0] != NULL && strcmp(args[0],"~") != 0)
            cd(args[0]);
        else
            cd(HOME);
    }
    else if(strcmp(comm,"pinfo") == 0)
    {
        if(args[0] != NULL)
        {
            long pid = strtol(args[0],NULL,10);
            output = pinfo(pid);
        }
        else
            output = pinfo(getpid());
        printf("%s",output);
    }
    else if(strcmp(comm,"ls") == 0)
    {
        if(args[0] == NULL)
        {
            ls(".",0);
        }
        else
        {
            if(args[1] == NULL) 
            {
                if(strcmp(args[0],"-a") == 0)
                    ls(".",1);
                else if(strcmp(args[0],"-l") == 0)
                    ls(".",2);
                else if(strcmp(args[0],"-al") == 0 || strcmp(args[0],"-la") == 0 )
                    ls(".",3);
            }
            else
            {
                if(!(strcmp(args[0],"-a") && !strcmp(args[1],"-l"))||(!strcmp(args[0],"-l") && !strcmp(args[1],"-a")))
                    ls(".",3);
            }
            
        }
    }
    else 
        printf("%s",comm);
    return; 
}
int main()
{
    HOME = getPWD();
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
            int j;
            char* mainComm = malloc(sizeof(char) * 25);
            char** args = malloc(sizeof(char*) * 10);
            int argN;
            argN = parseCommand(commQ[i],&mainComm,&args);
            checkBuiltIn(mainComm,args);
        //    printf("%s\n",mainComm);
          //  for (j = 0;j<argN;j++)
           //     printf("%s\n",args[j]);
           // printf("\n");
            //printf("%s\n",commQ[i]);
        }
        free(PWD);
        free(commQ);
       // printf("%s\n",pinfo(getpid()));
//      printf("%s\n",pinfo(12815));
        //cd("/home");
        //printf("%s",line);
    }
    while(1);
    return 0;
}
