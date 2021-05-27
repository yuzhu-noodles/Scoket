#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
//#include<arpe/inet.h>

#define MAXLINE 80
#define SERVER_PORT 8000


int main(){
	struct sockaddr_in servaddr,cliaddr;
	socklen_t cliaddr_len;
	int sockfd;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i,n;

	sockfd = socket(AF_INET,SOCK_DGRAM,0); //创建无连接套接字

	bzero(&servaddr,sizeof(servaddr)); //初始化为空
	servaddr.sin_family = AF_INET;  //采用ipv4地址
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 赋值ip地址为符号常量INADDR_ANY ,并将本地字节序转化为网络字节序
	servaddr.sin_port = htons(SERVER_PORT); // 端口号从主机字节序转化为网络字节序

	bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)); //将套接字绑定本机可用端点地址

	printf("等待连接...\n");
	while(1){
		cliaddr_len = sizeof(cliaddr);
		//接收客户端传来的字符串，写入buf数组
		n = recvfrom(sockfd,buf,MAXLINE,0,(struct sockaddr*)&cliaddr,&cliaddr_len);
		if(n == -1)
			printf("recvfrom error");

		//打印客户端IP和port
	    printf("接收到来自地址为 %s 端口号为 %d 的数据\n",inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
		for(i = 0;i<n;i++)
			buf[i] = toupper(buf[i]);     //小写转为大写
		//发送数据给客户端
		n = sendto(sockfd,buf,n,0,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
		if(n == -1)
			printf("sendto error\n");
	}

	return 0;

}
