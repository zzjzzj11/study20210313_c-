#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv )
{
	int server_sockfd,client_sockfd;
	int server_len,client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	int i , byte , ch_send;
	char recv_buf[128];
	char send_buf[128] = "ACK";
	
	unlink("server_socket");
	server_sockfd = socket(AF_UNIX,SOCK_STREAM,0);
	
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path,"server_socket" );
	server_len = sizeof(server_address);

	bind(server_sockfd, (struct sockaddr*)&server_address, server_len);	
	
	listen(server_sockfd,5);

	printf("server waiting for client connect\n");

	client_len = sizeof(client_address);
	
	client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address,(socklen_t*)&client_len);
	
	printf("the server wait from client data\n");

	for(i = 0, ch_send = 0; i < 5; i++ , ch_send++)
	{
		if ((byte = read(client_sockfd, recv_buf , sizeof(recv_buf))) == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		printf("the message receiver from client is: %s\n",recv_buf);
		sleep(1);
		
		if((byte= write(client_sockfd,&ch_send ,sizeof(ch_send))) == -1)
		{
			perror("write");
			exit(EXIT_FAILURE);
		}
	
	}
	close(client_sockfd);
	unlink("server_socket");
	exit(0);

}









