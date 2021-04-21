#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char* argv[])
{
    int fd = open("hello", O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);
    if(fd < 0)
    {
        printf("Open Error!!\n");
        return 0;
    }


    int nfd = dup(fd);
    if(nfd < 0)
    {
        printf("Error!!\n");
        return 0;
    }


    char buf[1000];
    int n;


    while((n = read(STDIN_FILENO, buf,1000)) > 0)
    {
        if(write(nfd, buf, n) != n)
        {
            printf("Write Error!!\n");
            return 0;
        }
    }
    return 0;


}