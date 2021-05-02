#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define MSGKEY 201844

struct msgstru{
	long msgtype;
	char msgtext[2048];

	};

int main()
{
	struct msgstru msg;
	int mtype;
	char str[256];
	int ret ; 
	int mqid;
	
	mqid = msgget(MSGKEY,IPC_EXCL);	
	if (mqid < 0)
	{
		mqid = msgget(MSGKEY,IPC_CREAT|0666);
		if(mqid < 0)
		{
		printf("create mq failed: errno = %d,%s\n",errno,strerror(errno));
		exit(-1);
		}
	}

	while(1)
	{
		printf("input msgtype:[input 0 to termnate]");
		scanf("%d",&mtype);
		if (mtype == 0)
		{
			break;
		}
		printf("input msgtext:");
		scanf("%s",str);
		msg.msgtype = mtype;
		strcpy(msg.msgtext, str);
		// if queue are full ,return -1
		ret = msgsnd(mqid , &msg ,sizeof(msgstru),IPC_NOWAIT);
		if (ret < 0)
		{
			printf("msgsnd() write msg failed, errno = %d,%s\n",errno,strerror(errno));
			exit(-1);
		}
	
	}
	msgctl(mqid, IPC_RMID , 0);
	return 0;

}























