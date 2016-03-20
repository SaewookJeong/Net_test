#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include <iostream>
#include <iomanip>
using namespace std;
   
int main(int argc, char *argv[])
{
   
	SOCKET clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo;
	char sendmessage[1024];
	char revmessage[1024];
   	char message[30];
	char message1[]="disconnected";
	char id2[]="Client";
	char id[30];
	int strlen;
   
	if(argc!=3)
	{
	   printf("Usage : %s <IP> <PORT> \n",argv[0]);
	   exit(1);
	} 

	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
	printf("초기화 실패\n");
   
	clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) ;
   
	if(clientsock == INVALID_SOCKET)
		printf("소켓 생성 실패\n");
   
	memset(&sockinfo,0,sizeof(sockinfo));
   
	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port =htons (atoi(argv[2]));
	sockinfo.sin_addr.s_addr =inet_addr(argv[1]) ;
   
	if(connect(clientsock,(SOCKADDR*)&sockinfo,sizeof(sockinfo))==SOCKET_ERROR) // 서버 측에 연결 요청
	printf(" 서버 접속 실패 ");

	strlen=recv(clientsock , message , sizeof(message)-1 , 0 );
	if(strlen==-1)
	printf(" 메세지 수신 실패 ");
	printf(" Server say: %s \n",message);

	while(1){
	
		recv(clientsock, id, sizeof(id), 0);
		cout<<" "<<id;

		recv(clientsock, revmessage, sizeof(revmessage), 0);
		if(strncmp(revmessage, "quit", 4) == 0)
		{
			closesocket(clientsock);		
			printf("%s", message1);
			goto end;
		}

		cout<<": "<<revmessage<<endl;
		cout<<" "<<id2<<": ";
		cin>>sendmessage;
		if(strncmp(sendmessage, "quit", 4) == 0)
		{
			closesocket(clientsock);		
			printf("%s", message1);
			goto end;
		} 

		send(clientsock, id2, sizeof(id2), 0);
		send(clientsock, sendmessage, sizeof(sendmessage), 0);
		char revmessage[1024]={0};
		char sendmessage[1024]={0};
		}

end:
	WSACleanup();
}