#include<dirent.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<pwd.h>
#include<stdlib.h>
#include<time.h>
void getPermissions(struct stat info,char* perm)
{
    int i;
    for(i=0;i<10;i++) perm[i] = '-';
    
    if(S_ISDIR(info.st_mode)) perm[0]='d';
    
    if(info.st_mode & S_IRUSR) perm[1]='r';
    if(info.st_mode & S_IWUSR) perm[2]='w';
    if(info.st_mode & S_IXUSR) perm[3]='x';

    if(info.st_mode & S_IRUSR) perm[4]='r';
    if(info.st_mode & S_IWUSR) perm[5]='w';
    if(info.st_mode & S_IXUSR) perm[6]='x';
   
    if(info.st_mode & S_IRUSR) perm[7]='r';
    if(info.st_mode & S_IWUSR) perm[8]='w';
    if(info.st_mode & S_IXUSR) perm[9]='x';
    return ;
    
}
char* getUser(struct stat info)
{
    char* ans = malloc(sizeof(char) * 25);
    struct passwd* buf;
    buf = getpwuid(info.st_uid);
    ans =  buf->pw_name;
    return ans;
}
char* getGroup(struct stat info)
{
    char* ans = malloc(sizeof(char) * 25);
    struct passwd* buf;
    buf = getpwuid(info.st_gid);
    ans =  buf->pw_name;
    return ans;
}
char* getLastModTime(struct stat info)
{
    int i,k=0;
    struct timespec t;
    t = info.st_mtim;
    char* time = ctime(&t.tv_sec);
    char* ans = malloc(sizeof(char)*13);
    for(i=4;i<=15;i++)
        ans[k++] = time[i];
    ans[k] = '\0';
    return ans;
}
int ls(char* path)
{
    // This shows hidden files also//
    // Effectively, this is 'ls -a'
    DIR* dp = opendir(path);
    struct dirent* readir;
    while((readir = readdir(dp)) != NULL)
    {
        // ls -a
        //printf("%s\t",readir->d_name);
        //printf("\n");
        // ls -al
        struct stat statBuf;
        stat(readir->d_name,&statBuf);
        
        char perm[10];
        getPermissions(statBuf,perm);
        printf("%s ",perm);
        
        char* user;
        user = getUser(statBuf);
        printf("%*s ",10,user);
        
        char* group;
        group = getGroup(statBuf);
        printf("%*s ",10,group);
        
        printf("%*ld ",10,statBuf.st_size);
        
        char* time;
        time = getLastModTime(statBuf);
        printf("%s ",time);
        
        printf("%s\n",readir->d_name);
        
        //printf("%ld\n",info.st_blksize);
        //printf("%ld\n",info.st_blocks);
    }
    closedir(dp);
    printf("\n");
    return 0;
}

int main()
{
    ls(".");
    //If no args are given, should take pwd as arg//
    return 0;
}
