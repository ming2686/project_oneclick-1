#pragma comment(lib,"ws2_32.lib")  // winsock2�� ����ϱ� ���� lib�� �߰��մϴ�.
#include <stdio.h>
#include <WinSock2.h>
#define DEFAULT_PORT "27015"

// ������ ������ ���� �⺻���� �帧
// Winsock �ʱ�ȭ -> socket ���� -> network ��� -> socket �ݱ� -> Winsock ����

int main() {

	//WSADATA ����ü���� Windows ���� ������ ���� ������ ���ԵǾ� ����
	WSADATA wsaData;
	SOCKET serverSocket;
	SOCKET clientSocket; // Ŭ���̾�Ʈ�� ������ �����ϱ� ���� �ӽ� SOCKET
	struct sockaddr_in server;

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

	server.sin_addr.s_addr = INADDR_ANY; // �ڵ����� �� ��ǻ�Ϳ� �����ϴ� ��ī�� �� ��밡���� ��ī���� IP�ּ� ���
	server.sin_family = AF_INET; // IPv4
	server.sin_port = htons(9090); // ����� ��Ʈ ��ȣ ����

	// 3. Socket ���� �� Bind
	if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code : %d\n", WSAGetLastError());
	}
	else {
		Sleep(2000);
		printf("Bind done!\n");
	}
	
	// 4. Socket���� Listen�� ���� ���� ���
	
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

	/*Sleep(2000);
	listen(serverSocket, SOMAXCONN);
	printf("Waiting for incoming connections...\n");*/


	// 5. Client�� ���� ��û�� �����ϱ�
	clientSocket = INVALID_SOCKET;
	clientSocket = accept(serverSocket,NULL,NULL);
	if (clientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	else {
		printf("Connection accepted!\n");
	}



}