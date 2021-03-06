#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define USER_LIMIT 5
#define BUFFER_SIZE 1024
#define FD_LIMIT 65535
#define MAX_EVENT_NUMBER 1024
#define PROCESS_LIMIT 65536

//处理一个客户连接必要到数据
struct client_data
{
    sockaddr_in address; //客户端的socket地址
    int connfd;          // socket文件描述符
    pid_t pid ;          //处理这个连接的子进程到pid
    int pipefd[2];       // 和父进程通信到管道
}

static const char * shm_name = "/my_shm";
int sig_pipefd[2];
int epollfd;
int listenfd;
int shmfd;
char *share_mem = 0 ;
client_data* users = 0;
int *sub_process = 0;
int user_count = 0;
bool stop_child = false;

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK ; 
    fcntl(fd , F_SETFL , new_option);
    return old_option;
}

void addfd(int epollfd , int fd)
{
    epoll_event event;
    event.data.fd = fd ;
    event.events = EPOLLIN | EPOLLET ;
    epoll_ctl(epollfd , EPOLL_CTL_ADD , fd ,&event);
    setnonblocking(fd);

}

void sig_handler(int sig)
{
    int save_errno =errno ;
    int msg = sig ;
    send(sig_pipefd[1], (char* )&msg, 1 , 0 );
    errno = save_errno;

}

void addsig(int sig , void(*handler)(int ), bool restart = true)
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if (restart)
    {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL ) != -1);

}

void del_resource()
{
    close(sig_pipefd[0]);
    close(sig_pipefd[1]);
    close(listenfd);
    close(epollfd);
    shm_unlink(shm_name )
    delete [] users;
    delete [] sub_process;
}

void child_term_handler(int sig)
{
    stop_child = true;

}

int run_child(int idx , client_data *users , char* share_mem)
{
    epoll_event events[MAX_EVENT_NUMBER];
    int child_epollfd = epoll_create(5);
    assert(child_epollfd != -1);
    int connfd = users[idx].connfd;
    addfd(child_epollfd , connfd);
    int pipefd = users[idx].pipefd[1];
    addfd(child_epollfd , pipefd);
    int ret ;

    addsig(SIGTERM , child_term_handler ,false);

    while(!stop_child)
    {
        int number = epoll_wait(child_epollfd , events, MAX_EVENT_NUMBER , -1);
        if ((number < 0) && (errno != EINTR))
        {
            printf("epoll failure \n");
            break;

        }
        for (int i =0; i < number ; i++)
        {
            int sockfd events[i].data.fd;
            if((sockfd = connfd) && (events[i].events & EPOLLIN))
            {
                memset(share_mem + idx*BUFFER_SIZE , '\0' , BUFFER_SIZE);
                ret = recv(connfd , share_mem + idx * BUFFER_SIZE , BUFFER_SIZE-1 , 0);
                if (ret < 0 )
                {
                    if(errno === EAGAIN)
                    {
                        stop_child = true;
                    }
                }else if (ret == 0)
                {
                        stop_child = true;
                }else
                {
                    send(pipefd , (char* )&idx ,sizeof(idx), 0);
                }

            }else if ((sockfd == pipefd) && (events[i ].events & EPOLLIN) )
            {
                int client = 0;
                ret = recv(sockfd , (char *)&client ,sizeof(client), 0);
                if(ret < 0)
                {
                    if(errno != EAGAIN)
                    {
                        stop_child = true;
                    }

                }else if (ret == 0)
                {
                    stop_child = true;
                }
                else
                {
                    send(connfd , share_mem + client*BUFFER_SIZE, BUFFER_SIZE , 0);

                }

            }else 
            {
                continue;
            }

        }
        
    }
    close(connfd);
    close(pipefd);
    close(child_epollfd);
    return 0;

}


int main()
{
    //未完待续
}
