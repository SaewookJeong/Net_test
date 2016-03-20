#include<stdio.h>
#include<winsock2.h>
#include <iostream>
#include <iomanip>
using namespace std;
#define BUFSIZE 512;

int main(int argc, char *argv[])
{
		SOCKET sock,clientsock;
		WSADATA wsa;
		struct sockaddr_in sockinfo, clientinfo;
		int clientsize;
		char message[]="connected";
		char message1[]="disconnected";
		char	id[]="server";
		char id2[30];
		char sendmessage[1024];
		char revmessage[1024];
		int len;

		if(argc != 2){
		printf("Usage : %s <port> <-echo>\n", argv[0]);
		exit(1);
		}
		
		
    
		if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		printf("초기화 실패\n");
   
		sock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
   
		if(sock == INVALID_SOCKET)
		printf("소켓 생성 실패\n");
   
		memset(&sockinfo,0,sizeof(sockinfo));
   
		sockinfo.sin_family = AF_INET;
		sockinfo.sin_port =htons (atoi(argv[1]));
		sockinfo.sin_addr.s_addr = htonl ( INADDR_ANY );    
   
		if ( bind ( sock , (SOCKADDR*)&sockinfo , sizeof(sockinfo) ) == SOCKET_ERROR )
		printf(" bind 실패 " );
	   
		if ( listen ( sock , 5 )==SOCKET_ERROR )
		printf(" 대기열 실패 ");
   
		clientsize = sizeof(clientinfo);
   
		printf("\n클라이언트로부터 접속을 기다리고 있습니다...\n\n");
 
		clientsock = accept(sock, (SOCKADDR*)&clientinfo, &clientsize);
   
		if(clientsock==INVALID_SOCKET)
		printf(" 클라이언트 소켓 연결 실패 ");

		send(clientsock, message, sizeof(message), 0 );
		cout<<"접속이 이루어 졌습니다\n";


		while(1){	
		cout<<" "<<id<<": "; 
		cin>>sendmessage;
		if(strncmp(sendmessage, "quit", 4) == 0)
		{
			closesocket(sock);
			closesocket(clientsock);		
			printf("%s", message1);
			break;
		}
		len = strlen(sendmessage);
		if(sendmessage[len-1] == '\n')
			sendmessage[len-1] = '\0';
		send(clientsock, id, sizeof(id), 0);
		send(clientsock, sendmessage, sizeof(sendmessage), 0);
		recv(clientsock, id2, sizeof(id2), 0);
		cout<<" "<<id2;
		recv(clientsock, revmessage, sizeof(revmessage), 0);
		cout<<": "<<revmessage<<endl;
		if(strncmp(revmessage, "quit", 4) == 0)
		{
			char revmessage[1024]={0};
			char sendmessage[1024]={0};
			closesocket(clientsock);		
			printf("%s", message1);
			break;
		}
	}
WSACleanup(); 
}