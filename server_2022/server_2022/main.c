#pragma comment(lib,"ws2_32.lib")  // winsock2�� ����ϱ� ���� lib�� �߰��մϴ�.
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>
#include <io.h>
#define DEFAULT_PORT "27015"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

// ������ ������ ���� �⺻���� �帧
// Winsock �ʱ�ȭ -> socket ���� -> network ��� -> socket �ݱ� -> Winsock ����

int main() {

	//WSADATA ����ü���� Windows ���� ������ ���� ������ ���ԵǾ� ����
	WSADATA wsaData;
	SOCKET serverSocket;
	SOCKET newSocket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char *message = NULL;
	int c;

	// 1. Winsock �ʱ�ȭ, WINSOCK VERSION 2.2
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed: %d\n", WSAStartup(MAKEWORD(2, 2), &wsaData));
		return 1;
	}
	else {
		Sleep(2000);
		printf("WSAStartup completed!!\n");
	}

	// 2. �ʱ�ȭ �� �������� ����� SOCKET �ν��Ͻ� ����
	// socket() function is used to create a socket
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d\n", WSAGetLastError());
	}
	else {
		Sleep(2000);
		printf("Socket has been created!\n");
		// Address Family : AF_INET ( = IPv4)
		// Type : SOCK_STREAM( = TCP Protocol connection)
		// Protocol : 0
	}

	//server.sin_addr.s_addr = INADDR_ANY; // �ڵ����� �� ��ǻ�Ϳ� �����ϴ� ��ī�� �� ��밡���� ��ī���� IP�ּ� ���
	server.sin_addr.s_addr = inet_addr("211.215.249.35");
	server.sin_family = AF_INET; // IPv4
	server.sin_port = htons(8888); // ����� ��Ʈ ��ȣ ����

	// 3. Socket ���� �� Bind
	if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code : %d\n", WSAGetLastError());
	}
	else {
		Sleep(2000);
		printf("Bind done!\n");
	}

	// 4. Socket���� Listen�� ���� ���� ���
	/*Sleep(2000);
	listen(serverSocket, SOMAXCONN);
	printf("Waiting for incoming connections...\n");*/

	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	else {
		Sleep(2000);
		printf("Waiting for incoming connections...\n");
	}

	// 5. Client�� ���� ��û�� accept�ϱ�
	c = sizeof(struct sockaddr_in);
	newSocket = INVALID_SOCKET;
	
	while ((newSocket = accept(serverSocket, NULL, NULL))!=INVALID_SOCKET) {
		printf("Connection accepted!\n");
		char* recv_buf = malloc(sizeof(char)*100);
		//message = malloc(sizeof(char) * 1000);
		int recv_size;
		// 5.1 Client�κ��� �� Data Read
		recv_size = recv(newSocket, recv_buf, strlen(recv_buf), 0);
		recv_buf[recv_size] = '\0';
		printf("Message from Client : %s\n", recv_buf);

		// 6. Reply to Client
		//strcpy(message, recv_buf);
		message = "Hello client! ";
		send(newSocket, message, strlen(message), 0);
		free(recv_buf);
		//free(message);
	}
	if (newSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}


	closesocket(newSocket);
	closesocket(serverSocket);
	WSACleanup();

}