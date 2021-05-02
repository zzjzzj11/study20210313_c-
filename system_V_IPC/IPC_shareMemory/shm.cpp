#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>
int main(int argc,char *argv[])
{
 key_t shmid;
 char *p_addr,*c_addr;
 pid_t pid;
 if(argc != 2)
 {
  printf("Usage:program something",argv[0]);
  exit(0);
 }
 if((shmid = shmget(IPC_PRIVATE,1024,S_IRUSR|S_IWUSR)) == -1)
 {
  printf("create share memory error/n");
  exit(1);
 }
 pid = fork();
 if(pid > 0 )
 {
  p_addr = (char*)shmat(shmid,0,0);
  memset(p_addr,'/0',1024);
  strncpy(p_addr,argv[1],1024);
  wait(NULL);
 }
 else if (pid == 0)
 {
  sleep(10);
  c_addr = (char*)shmat(shmid,0,0);
  printf("Client get %s/n",c_addr);
  exit(0);
 }
 if(shmctl(shmid,IPC_RMID,0)<0)
 {
  printf("共享内存释放出错!!/n");
  return(-1);
 }
 else
  printf("释放共享内存成功!/n");
}