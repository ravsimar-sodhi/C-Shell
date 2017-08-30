#include "ls.c"
#include "pwd.c"
#include "cd.c"
#include "user@host.c"
#include "pinfo.c"
#include <signal.h>
#include <string.h>
#include <wait.h>
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
    char* token = strtok(fullComm," \t\n");
    *mainComm = token;
    while(token != NULL)
    {
        token = strtok(NULL," \t\n");
        (*args)[i++] = token;
    }
    return i-1;
}
int checkBuiltIn(char* comm,char** args)
{
   // if(comm == "ls")
    char* output;
    if(strcmp(comm,"pwd") == 0)
    {
        output = getPWD();
        printf("%s\n",output);
        return 1;
    }
    else if(strcmp(comm,"cd") == 0)
    {
        if(args[0] != NULL && strcmp(args[0],"~") != 0)
            cd(args[0]);
        else
            cd(HOME);
        return 1;
    }
    else if(strcmp(comm,"pinfo") == 0)
    {
        if(args[0] != NULL)
            output = pinfo(atoi(args[0]));
        else
            output = pinfo(getpid());
        printf("%s",output);
        return 1;
    }
    else 
        return 0;
}

void child_terminate()
{
        union wait wstat;
        pid_t   pid;

        while (1) {
            pid = wait3 (&wstat, WNOHANG, (struct rusage *)NULL );
            if (pid == 0)
                return;
            else if (pid == -1)
                return;
            else
                fprintf (stderr,"Process with id: %d terminted %s\n", pid,(wstat.w_retcode==0)?"normally":"abnormally");
        }
}

int main()
{
    signal(SIGCHLD,child_terminate);
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
            if(strcmp(mainComm,"exit")==0)
                exit(0);
            int builtin = checkBuiltIn(mainComm,args);
            if(!builtin)
            {
                pid_t pid;
                pid=fork();
                if(pid==0)
                {
                    char *exec_arr[2];
                    exec_arr[0]=mainComm;
                    exec_arr[1]=NULL;
                    execvp(mainComm,exec_arr);
                    printf("Failed to execute\n");
                    exit(0);
                }
                else
                {
                    if(args[0]==NULL || strcmp(args[0],"&")!=0)
                        wait(NULL);
                    else
                        printf("%s [%d] started\n",mainComm,pid);
                }
            }
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
