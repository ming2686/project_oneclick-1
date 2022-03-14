#pragma comment(lib,"ws2_32.lib")  // winsock2�� ����ϱ� ���� lib�� �߰��մϴ�.
#include <stdio.h>
#include <WinSock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define DEFAULT_PORT "27015"
#define MY_LOCAL_IP "211.215.249.35"

// Client�� ���� ����
// 1. ���� ���� -> 2. ������ Connect -> 3. Send data -> 4. Recieve Reply

int main() {
	
	WSADATA wsaData;
	SOCKET clientSocket;
	struct sockaddr_in server;

	// initializing Winsock
	Sleep(2000);
	printf("Initializing Winsock...\n");
	// 1. Winsock �ʱ�ȭ, WINSOCK VERSION 2.2
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed: %d\n", WSAStartup(MAKEWORD(2, 2), &wsaData));
		return 1;
	}
	else {
		Sleep(2000);
		printf("WSAStartup completed!!\n");
	}

	// 2. Creating socket
	// 2. �ʱ�ȭ �� �������� ����� SOCKET �ν��Ͻ� ����
	// socket() function is used to create a socket
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d\n", WSAGetLastError());
	}
	else {
		Sleep(2000);
		printf("Socket has been created!\n");
		// Address Family : AF_INET ( = IPv4)
		// Type : SOCK_STREAM( = TCP Protocol connection)
		// Protocol : 0
	}

	server.sin_addr.s_addr = inet_addr(MY_LOCAL_IP); // ���� IP
	server.sin_family = AF_INET; // IPv4
	server.sin_port = htons(9090); // ����� ��Ʈ ��ȣ ����

	// 3. Connect to server
	if (connect(clientSocket, (struct sockaddr*)&server, sizeof(server)) < 0) {
		printf("Connect error!\n");
		return 1;
	}
	else {
		printf("Connection has been completed!\n");
	}

}