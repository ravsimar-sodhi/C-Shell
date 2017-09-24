#include "headers.h"
char* HOME;
typedef struct process
{
    int proid;
    char *name;
    char *state;
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
    char* output;
    if(strcmp(comm,"pwd") == 0)
    {
        output = getPWD();
        printf("%s\n",output);
        return 1;
    }
    else if(strcmp(comm,"cd") == 0)
    {

        int i,k=strlen(HOME);
        if(args[0] != NULL && strcmp(args[0],"~") != 0)
        {    
            if(strstr(args[0],"~") != NULL)
            {
                char* fullPath = malloc(sizeof(char) * 120);
                strcpy(fullPath,HOME);
                strcat(fullPath,&args[0][1]);
                cd(fullPath);
            }
            else
                cd(args[0]);
        }
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
    else if(strcmp(comm,"kjobs") == 0)
    {
    	kjobs(args);
        return 1;
    }
    else if(strcmp(comm,"setenv") == 0)
    {
        if(args[0] == NULL)
            printf("Error:Name of variable not found\n");
        else if(args[2] != NULL)
            printf("Error: Too many arguements\n");
        else if(args[1] != NULL)
            setenv(args[0],args[1],1);
        else 
            setenv(args[0],"",1);
        return 1;
    }
    else if(strcmp(comm,"getenv") == 0)
    {
        char* out = NULL;
        if(args[0] == NULL)
            printf("Error: Name of variable not found\n");
        else
            out = getenv(args[0]);
        if(out == NULL)
            printf("Error: variable not found in env\n");
        else
            printf("%s\n",out);
        return 1;
    }
    else if(strcmp(comm,"unsetenv") == 0)
    {
        if(args[0] == NULL)
            printf("Error: Name of variable not found\n");
        else
        {
            char* out = NULL;
            out = getenv(args[0]);            
            if(out == NULL)
                printf("Error: variable not found in env\n");
            else if(unsetenv(args[0]) == -1)
                printf("Error: variable not found in env");
        }
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
                fprintf (stderr,"%s with pid: %d terminated %s\n", pname,pid,(wstat.w_retcode==0)?"normally":"abnormally");
            }
        }
}

int main()
{
    signal(SIGCHLD,child_terminate);
    HOME = getPWD();
    int i;
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
            char** args = malloc(sizeof(char*) * 100);
            int argN;
            argN = parseCommand(commQ[i],&mainComm,&args);
            if(strcmp(mainComm,"exit")==0 || strcmp(mainComm,"quit")==0)
                exit(0);
            else if(strcmp(mainComm,"ls") == 0)
            {
                ls(args,argN,HOME);
            }
            else if(strcmp(mainComm,"nightswatch")==0)
            {
                if(argN!=3 || strcmp(args[0],"-n")!=0)
                    fprintf(stderr,"Correct usage: nightswatch -n <seconds> <command>\n");
                else
                {
                    int t = atoi(args[1]);
                    nightswatch(t,args[2]);
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
                        exec_arr[0] = mainComm;
                        int z,z1=1;
                        for (z=0;z<=argN;z++)
                        {
                            if(args[z]==NULL || strcmp(args[z],"&")!=0)
                            {
                                exec_arr[z1] = args[z];
                                z1++;
                            }
                        }
                        execvp(mainComm,exec_arr);
                        fprintf(stderr,"Command Not Found\n");
                        exit(0);
                    }
                    else
                    {
                        if(args[0]==NULL || strcmp(args[argN-1],"&")!=0)
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
