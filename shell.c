#include "ls.c"
#include "pwd.c"
#include "cd.c"
#include "user@host.c"
#include "pinfo.c"
#include <signal.h>
#include <string.h>
#include <wait.h>
#include "echo.c"
char* HOME;
typedef struct process
{
    int proid;
    char *name;
}process;
process dict[100];
int procNo=0;
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
    if(strcmp(*mainComm,"echo") == 0)
    {
        token = strtok(NULL,";");
        (*args)[0] =  token;
        return 1;
    }
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
    else if(strcmp(comm,"echo") == 0)
    {
        output = echo(args[0]);
        printf("%s\n",output);
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
        char *pname=malloc(100);
        while (1) {
            pid = wait3 (&wstat, WNOHANG, (struct rusage *)NULL );
            if (pid == 0)
                return;
            else if (pid == -1)
                return;
            else
            {
                int i;
                for(i=0;i<procNo;i++)
                {
                    if(dict[i].proid==pid)
                        pname=dict[i].name;
                } 
                fprintf (stderr,"%s with pid: %d terminted %s\n", pname,pid,(wstat.w_retcode==0)?"normally":"abnormally");
            }
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
        if(strstr(fullPWD,HOME) != NULL)
        {
            PWD[0] = '~';
            PWD[1] = '\0';
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
            if(strcmp(mainComm,"exit")==0 || strcmp(mainComm,"quit")==0)
                exit(0);
            else if(strcmp(mainComm,"ls") == 0)
            {
                struct stat a;
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
                        else if(!stat(args[0],&a))
                        {
                            if(S_ISDIR(a.st_mode))
                                ls(args[0],0);
                            else
                                printf("Invalid Directory\n");
                        }
                    }
                    else
                    {
                        if((strcmp(args[0],"-a") == 0 && strcmp(args[1],"-l") == 0)||(strcmp(args[0],"-l") == 0 && strcmp(args[1],"-a") == 0))
                            ls(".",3);
                        else
                        {
                            int flag = 0;
                            if(strcmp(args[0],"-a") == 0) flag = 1;
                            else if(strcmp(args[0],"-l") == 0) flag = 2;
                            else if(strcmp(args[0],"-al") == 0) flag = 3;
                            
                            if(!stat(args[1],&a))
                            {
                                if(S_ISDIR(a.st_mode))
                                    ls(args[1],flag);
                                else
                                    printf("Invalid Directory\n");
                            }
                        }
                    }
                }
            }
            else
            {
                int builtin = checkBuiltIn(mainComm,args);
                if(!builtin)
                {
                    pid_t pid;
                    pid=fork();
                    if(pid==0)
                    {
                        char *exec_arr[20];
                        exec_arr[0]=mainComm;
                        int z;
                        for (z=0;z<=argN;z++)
                        {
                            exec_arr[z+1]=args[z];
                        }
                        execvp(mainComm,exec_arr);
                        printf("Failed to execute\n");
                        exit(0);
                    }
                    else
                    {
                        if(args[0]==NULL || strcmp(args[0],"&")!=0)
                            wait(NULL);
                        else
                        {
                            dict[procNo].proid = pid;
                            dict[procNo].name = mainComm;
                            procNo++;
                            printf("%s [%d] started\n",mainComm,pid);
                        }
                    }
                }
            }
        }
        free(PWD);
        free(commQ);
    }
    while(1);
    return 0;
}
