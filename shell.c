#include "ls.c"
#include "pwd.c"
#include "cd.c"
#include "user@host.c"
char* getInput()
{
    ssize_t read;
    char* buffer = NULL;
    size_t bufsize = 0;
    while(1)
    {
        read = getline(&buffer,&bufsize,stdin);
        if(read != -1)
            return buffer;
        else
            break;
    }
}
int main()
{
    do
    {
        printf("<%s@%s:>",getUserName(),getHostName());
        char* line = getInput();
        printf("%s",line);
    }
    while(1);
    return 0;
}
