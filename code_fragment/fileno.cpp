#include <stdio.h> 
   
int main(int argc, char **argv) 
   
{ 
    FILE * fp; 
    int fd; 
    fp = fopen("/etc/passwd", "r"); 
    fd = fileno(fp); 
    //等价于  
    //fd=open("etc/passwd",RD_ONLY);
    printf("fd=%d\n", fd); 
    fclose(fp); 
    return 0; 
}