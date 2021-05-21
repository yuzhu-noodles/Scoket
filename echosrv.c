#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#define ERR_EXIT(m)\
        do{\
			perror(m);\
			exit(EXIT_FAILURE);\
		}while(0)

int main(){

	int listenfd;  //监听描述符 fd file discriptor
	if((listenfd = socket(PF_INET,SOCK_STREAM,0))<0)
		ERR_EXIT("socket");
    
	//地址的初始化
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);  //端口号两个字节
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // INADDR_ANY 本机的任意地址  
//  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//  inet_aton("127.0.0.1",&servaddr.sin_addr);

	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
		ERR_EXIT("bind");
	if(listen(listenfd,SOMAXCONN)<0)   //成为主动套接字
		ERR_EXIT("listen");
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peerlen);
	int conn;  //已连接套接字  是主动套接字
	if((conn = accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen))<0)
		ERR_EXIT("accept");

    char recvbuf[1024];
	while(1){
		memset(recvbuf,0,sizeof(recvbuf));
		int ret = read(conn,recvbuf,sizeof(recvbuf));
		fputs(recvbuf,stdout);
		write(conn,recvbuf,ret);
	}
    
	close(conn);
	close(listenfd);
	return 0;
}
