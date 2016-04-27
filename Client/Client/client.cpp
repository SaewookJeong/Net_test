#include<stdio.h>
#include<winsock2.h>
#include<iostream>
#include<string.h>
#include<process.h> //�����带 ���� �������
 
using namespace std;

DWORD WINAPI ProcessClient(LPVOID arg)
{
   SOCKET clientsock = (SOCKET)arg;
   char sendmessage[1024];
   SOCKADDR_IN clientinfo;
   int clientsize; 
   int strlen; //recv ���� �� ���� �� ���� ����
   clientsize = sizeof(clientinfo); 
   /*
   getpeername(clientsock, (SOCKADDR*)&clientinfo, &clientsize);
   //�Լ��� ������ ����Ǿ� �ִ� ������ ����(peer)�� �̸�(�������̽� ��巹���� ��Ʈ��ȣ)�� ��� �Լ��Դϴ�.
   */
   while(true){
		for(int i = 0; i < 1024; i ++)sendmessage[i]=0;
		strlen=recv(clientsock , sendmessage, sizeof(sendmessage), 0);
		if(strlen == -1) {
			cout<<"�޽��� ���� ����"<<endl;
			break;
		}
		cout<<sendmessage<<endl;
   }
	return 0;
	//ExitThread((DWORD)ProcessClient);
}

int main(int argc, char *argv[])
{
	SOCKET clientsock; // Ŭ���̾�Ʈ ���α׷� ���� ����
	WSADATA wsa;
	struct sockaddr_in sockinfo; // ���� ���� �� �ּ� ����ü ����
 
	char id[10] = "Client";
	char sendmessage[1024];
	int result; // ��� ��
	
	if (argc != 3)
	{
		printf("Usage : %s <IP> <PORT> \n", argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		cout<<"�ʱ�ȭ ����\n"<<endl;

	clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(clientsock == INVALID_SOCKET)
		cout<<"���� ���� ����\n"<<endl;

	memset(&sockinfo, 0, sizeof(sockinfo)); // �ּ� ����ü �Լ� �ʱ�ȭ
  
	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port =htons(atoi(argv[2]));
	sockinfo.sin_addr.s_addr =inet_addr(argv[1]);

	result = connect(clientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)); // return ���� 0�̴�.  �� �ܿ��� SOCKET_ERROR ��(-1)�� �����Ѵ�.
	if(result == -1 || result == SOCKET_ERROR)
		cout<<"������ connect(���� ����) ��û�� �����Ͽ����ϴ�"<<endl;
	//cout<<result; // ���⼭ ��� ���� -1�̸� ������ ���� �ʴ� ��

	//strlen=recv(clientsock , message, sizeof(message)-1 , 0); // success = return value is zero. Otherwise = SOCKET_ERROR
	//if(strlen == -1) cout<<"�޽��� ���� ����"<<endl;
	cout<<"Connect"<<endl;

	HANDLE hThread;      // ������ �ڵ�
	DWORD ThreadID;     // ������ ���̵�
	
	hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)clientsock, 0, &ThreadID);
	while(1){
	//for(int i = 0; i < 1024; i ++) sendmessage[i]=0;
	cin>>sendmessage;
	if((strcmp(sendmessage, "quit") == 0))
	{
		cout<<"Disconnect"<<endl;
		break;
	}
	send(clientsock, sendmessage, sizeof(sendmessage)-1, 0);
	Sleep(10);
	}
	
	closesocket(clientsock);
 
	WSACleanup(); //���� ����

	return 0;
 
}

