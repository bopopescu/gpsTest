#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <asm/types.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include "gpsTest.h"
#include "manager.h"
#include "internetConnectCheck.h"
#include "recvSession.h"

int mainCommand = 0;

void getMainCommand()
{
    
}


void disPatchCommand(int command, int port)
{
		static int sockfd=0;
		struct sockaddr_in servaddr;

		
		bzero(&servaddr,sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);
		
		inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
		sockfd=socket(AF_INET,SOCK_DGRAM,0);

		sendto(sockfd, &command,sizeof(int),
			0,(struct sockaddr *)&servaddr,sizeof(struct sockaddr));
}

int handleRemoteCommand(unsigned char *data, unsigned short dataLen)
{
    pushCommandData_t recvData;
    return recvFromRemote(data, dataLen, &recvData);
}

int main()
{

    int period = 100000;//0.1 sec
    //unsigned long count = 0;
    struct timeval timeout;
	fd_set set;
    char command[12];
    int i = 0;
    pthread_t internetConnectCheck_id;
    int s = 0;
    int sock_opt = 1;
    static struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    char remotePushCommand[128];

    pthread_create(&internetConnectCheck_id, NULL, internetConnectCheck, NULL);

    
  
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0){
        printf("error to get socket\n");
        return -1;
    }
    sock_opt = 1;

    if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(void *)&sock_opt, sizeof(sock_opt)) == -1)
    {
            printf("error to set sock opt reuseaddr\n");
            return -1;

    }

            
    memset((char *)&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    //inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr);
    serv_addr.sin_port = htons(9998);
    

    if (bind (s, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("error to binding 9998\n");
        close(s);
        return -1;
    }


    for(;;)
	{
        int n = 0;
		FD_ZERO(&set);
		FD_SET(0,&set);
        FD_SET(s, &set);
		timeout.tv_sec=0;
		timeout.tv_usec=100000;
        memset(command, 0, 12);
        memset(remotePushCommand, 0, 128);
   
       
        select(FD_SETSIZE,&set,NULL,NULL,&timeout);
		

        if (FD_ISSET(0, &set)) 
        {
            //command = gets(stdin);
            if (fgets(command, LINE_MAX, stdin) != NULL) 
            {
                if('6' == command[0])
                {
                    printf("next stop\r\n");
                    disPatchCommand(NEXT_STOP_ANNOUNCE, PORT_ANNOUNCE);
                }
                else if('4' == command[0])
                {
                    printf("prev stop\r\n");
                    disPatchCommand(PREV_STOP_ANNOUNCE, PORT_ANNOUNCE);
                }
                else
                {
                    printf("unknow command\r\n");
                }
            }
            
        } 

        if(FD_ISSET(s, &set))
        {
            n=recvfrom(s, remotePushCommand, 128, 0, (struct sockaddr *)&cli_addr, &clilen);
            printf("recv remote push command data length =  %d\r\n", n);
            handleRemoteCommand(remotePushCommand, n);
        }    
        
    }
}