#include<stdio.h>
#include<winsock2.h>
#include<iostream>
#include<process.h> //�����带 ���� �������
using namespace std;
 
char echo[10];

DWORD WINAPI ProcessClient(LPVOID arg)
{
   SOCKET clientsock = (SOCKET)arg;
   char revmessage[1024];
   SOCKADDR_IN clientinfo;
   int clientsize;
   int strlen;
   clientsize = sizeof(clientinfo);
   getpeername(clientsock, (SOCKADDR*)&clientinfo, &clientsize);
   //�Լ��� ������ ����Ǿ� �ִ� ������ ����(peer)�� �̸�(�������̽� ��巹���� ��Ʈ��ȣ)�� ��� �Լ��Դϴ�.
 
   if(clientsock==INVALID_SOCKET) cout<<"Ŭ���̾�Ʈ ���� ���� ���� "<<endl;
	cout<<"Connect"<<endl;
		while(1){
		for(int i = 0; i < 1024; i ++) revmessage[i]=0;
		strlen=recv(clientsock , revmessage, sizeof(revmessage)-1, 0);
		if(strlen == 0) 
		{
			cout<<"Disconnect"<<endl;
			break;
		}
			cout<<revmessage<<endl;
			if(strcmp(echo, "-echo") == 0)
			send(clientsock, revmessage, sizeof(revmessage)-1, 0);
		}
	return 0;
}


int main(int argc, char *argv[])
{
	SOCKET sock, clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo, clientinfo;
	int clientsize;
	char message[]="Connect";

	if (argc != 2)
	{
		if(argc == 3 && strcmp(argv[2], "-echo") == 0)
		{
			strcpy(echo, argv[2]);
			goto start;
		}
		printf("Usage : %s <PORT> <-echo> \n", argv[0]);
		exit(1);
	}

start:
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		cout<<"�ʱ�ȭ ����"<<endl;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(sock == INVALID_SOCKET)
		cout<<"���� ���� ����"<<endl;

	memset(&sockinfo,0,sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(atoi(argv[1]));
	sockinfo.sin_addr.s_addr=htonl(INADDR_ANY);
  
	if(bind(sock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR) // return �� success = 0, otherwise = SOCKET_ERROR;
		cout<<"bind ����"<<endl;

	if(listen(sock , 5) == SOCKET_ERROR) // return �� success = 0, otherwise = SOCKET_ERROR;
		cout<<"��⿭ ���� "<<endl;
	
	clientsize = sizeof(clientinfo);
	cout<<"Ŭ���̾�Ʈ�κ��� ������ ��ٸ��� �ֽ��ϴ�.."<<endl;


	HANDLE hThread;      // ������ �ڵ�
	DWORD ThreadID;     // ������ ���̵�
		
	while(1) 
	{
		clientsock = accept(sock, (SOCKADDR*)&clientinfo, &clientsize);
		
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)clientsock, 0, &ThreadID);

		if(hThread == NULL)cout<<"������ ���� ����"<<endl;

		else CloseHandle(hThread);
    }	
	closesocket(clientsock);
	closesocket(sock);
	WSACleanup();
}