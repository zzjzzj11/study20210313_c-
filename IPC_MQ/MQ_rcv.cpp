#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <wait.h>
#define MSGKEY 201844

struct msgstru{
		long msgtype;
		char msgtext[2048];

	};

void childproc()
{
	struct msgstru msg;
	int mqid , ret ;
	char str[512];

	while(1)
	{
		mqid = msgget(MSGKEY,IPC_EXCL);
		if (mqid < 0)
		{
			printf("msg not exited! errno= %d [%s]\n",errno,strerror(errno));
			sleep(3);
			continue;	

		}
		ret = msgrcv(mqid , &msg, sizeof(msgstru), 1, 0);
		printf("pid = [%d] mtype= [%ld],mtext=[%s] \n",getpid(),msg.msgtype,msg.msgtext);
			
	}

	exit(0);

}

int main()
{
	int i , cpid;
	cpid = 0;

	#if 1
	for(i = 0 ; i < 5; i++)
	{
		cpid= fork();
		if (cpid < 0)
		{
			printf("fork failed");		
		}
		else if (cpid == 0)
		{
			childproc();
			break;
		}
	}
	#else
	childproc();
	#endif
	wait(NULL);
	return 0;


}






























