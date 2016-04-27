#include<stdio.h>
#include<winsock2.h>
#include<iostream>
#include<string.h>
#include<process.h> //스레드를 위한 헤더파일
 
using namespace std;

DWORD WINAPI ProcessClient(LPVOID arg)
{
   SOCKET clientsock = (SOCKET)arg;
   char sendmessage[1024];
   SOCKADDR_IN clientinfo;
   int clientsize; 
   int strlen; //recv 리턴 값 저장 할 변수 선언
   clientsize = sizeof(clientinfo); 
   /*
   getpeername(clientsock, (SOCKADDR*)&clientinfo, &clientsize);
   //함수는 소켓이 연결되어 있는 원격지 상대방(peer)의 이름(인터페이스 어드레스와 포트번호)을 얻는 함수입니다.
   */
   while(true){
		for(int i = 0; i < 1024; i ++)sendmessage[i]=0;
		strlen=recv(clientsock , sendmessage, sizeof(sendmessage), 0);
		if(strlen == -1) {
			cout<<"메시지 수신 실패"<<endl;
			break;
		}
		cout<<sendmessage<<endl;
   }
	return 0;
	//ExitThread((DWORD)ProcessClient);
}

int main(int argc, char *argv[])
{
	SOCKET clientsock; // 클라이언트 프로그램 소켓 생성
	WSADATA wsa;
	struct sockaddr_in sockinfo; // 서버 연결 할 주소 구조체 생성
 
	char id[10] = "Client";
	char sendmessage[1024];
	int result; // 결과 값
	
	if (argc != 3)
	{
		printf("Usage : %s <IP> <PORT> \n", argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		cout<<"초기화 실패\n"<<endl;

	clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(clientsock == INVALID_SOCKET)
		cout<<"소켓 생성 실패\n"<<endl;

	memset(&sockinfo, 0, sizeof(sockinfo)); // 주소 구조체 함수 초기화
  
	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port =htons(atoi(argv[2]));
	sockinfo.sin_addr.s_addr =inet_addr(argv[1]);

	result = connect(clientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)); // return 값은 0이다.  그 외에는 SOCKET_ERROR 값(-1)을 리턴한다.
	if(result == -1 || result == SOCKET_ERROR)
		cout<<"서버측 connect(서버 접속) 요청에 실패하였습니다"<<endl;
	//cout<<result; // 여기서 결과 값이 -1이면 연결이 되지 않는 것

	//strlen=recv(clientsock , message, sizeof(message)-1 , 0); // success = return value is zero. Otherwise = SOCKET_ERROR
	//if(strlen == -1) cout<<"메시지 수신 실패"<<endl;
	cout<<"Connect"<<endl;

	HANDLE hThread;      // 스레드 핸들
	DWORD ThreadID;     // 스레드 아이디
	
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
 
	WSACleanup(); //윈속 종료

	return 0;
 
}

