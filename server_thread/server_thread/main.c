#pragma comment(lib,"ws2_32.lib")  // winsock2�� ����ϱ� ���� lib�� �߰��մϴ�.
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>
#include <string.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9090
#define MAX_CLIENTS 20

HANDLE hMutex = 0; // ������ ����ȭ ó���� ���� ���ؽ�
int client_num = 0; // Ŭ���̾�Ʈ ����
int seat = 0; // Ŭ���̾�Ʈ ��ȣ
SOCKET client_sock[65535]; // Ŭ���̾�Ʈ welcom socket

// ���� �ʱ�ȭ
int server_init() {
	WSADATA wsaData;
	SOCKET s;
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
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	else {
		Sleep(2000);
		printf("Socket has been created!\n");
		// Address Family : AF_INET ( = IPv4)
		// Type : SOCK_STREAM( = TCP Protocol connection)
		// Protocol : 0
	}

	//server.sin_addr.s_addr = INADDR_ANY; // �ڵ����� �� ��ǻ�Ϳ� �����ϴ� ��ī�� �� ��밡���� ��ī���� IP�ּ� ���
	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET; // IPv4
	server.sin_port = htons(SERVER_PORT); // ����� ��Ʈ ��ȣ ����

	// 3. Socket ���� �� Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code : %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	else {
		Sleep(2000);
		printf("Bind done!\n");
	}

	// 4. Socket���� Listen�� ���� ���� ���
	if (listen(s, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return 1;
	}
	else {
		Sleep(2000);
		printf("Waiting for incoming connections...\n");
	}
	return s;
	
}

// ������ �Լ�
void recv_client(void* ns) {
	// ���������� �����û accpet �Ǿ��� �� ������ ����
	// Ŭ���̾�Ʈ�� ���ڸ� �ø�

	// �Ӱ� ����, mutex lock�� Ȱ���� ���� ���� �Ұ����ϰ� �Ѵ�
	WaitForSingleObject(hMutex, INFINITE);
	client_num++; // Ŭ���̾�Ʈ ���� ����
	seat++; // Ŭ���̾�Ʈ ��ȣ ����
	ReleaseMutex(hMutex);

	char welcome[100] = { 0 };
	char buff[1024] = { 0 };
	int ret, i;

	_itoa(seat, welcome, 10);
	strcat(welcome, "�� Ŭ���̾�Ʈ, ȯ���մϴ�\n");
	ret = send(*(SOCKET*)ns, welcome, sizeof(welcome), 0); // �޽��� ����

	while (ret != SOCKET_ERROR || ret != INVALID_SOCKET) {
		ret = recv(*(SOCKET*)ns, buff, 1024, 0); //Ŭ���̾�Ʈ�κ��� �޽��� ����

		// ��� client���� broadcast
		for (i = 0; i < 65535; i++) {
			// ���� Ŭ���̾�Ʈ ������ �޸� �ּҿ�
			// ������ Ŭ���̾�Ʈ�� ���� �޸� �ּҰ� �ٸ��� ����
			WaitForSingleObject(hMutex, INFINITE);
			if ((unsigned*)&client_sock[i] != (SOCKET*)ns) {
				send(client_sock[i], buff, strlen(buff), 0);
			}
			ReleaseMutex(hMutex);
		}
		if (strlen(buff) != 0) {
			printf("%d�� �޽��� ���� : %s", strlen(buff), buff);
		}
		memset(buff, 0, 1024);
	}

	// ���ӵ� ������ ������ ���� �������� 
	WaitForSingleObject(hMutex,INFINITE);
	client_num--;
	ReleaseMutex(hMutex);
	ReleaseMutex(hMutex);
	return;
}

int main() {
	int addrlen,addrsize;
	SOCKET serverSocket;
	struct sockaddr_in client;
	hMutex = CreateMutex(NULL, FALSE, NULL);
	if (!hMutex) {
		printf("Mutex ERROR\n");
		CloseHandle(hMutex);
		return 1;
	}

	serverSocket = server_init();
	if (serverSocket < 0) {
		printf("���� �ʱ�ȭ ����\n");
		exit(0);
	}
	// wait clients' connecting signal

	addrlen = sizeof(struct sockaddr_in);

	while (1) {
		//Blocking ������� Client ���
		client_sock[seat] = accept(serverSocket, (struct sockaddr*)&client, (int*) & addrlen);
		if (client_num < MAX_CLIENTS) {
			if (!(client_sock[seat] == INVALID_SOCKET || client_sock[seat] == SOCKET_ERROR)){
				_beginthread(recv_client, 0, &client_sock[seat]);
				Sleep(10);
				printf("New connection, %dth client, ip is : %s, port : %d\n", seat, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
			}
		}
		// ���� �� ���
		else {
			addrsize = sizeof(client);
			// client_sock �迭�� ���� ���� �� �̻� ���� �� ����
			if (client_sock[seat] == INVALID_SOCKET) {
				printf("accept error");
				closesocket(client_sock[seat]);
				closesocket(serverSocket);
				WSACleanup();
				return 1;
			}
			char* message = "Client Full!\n";
			if (send(client_sock[seat], message, strlen(message), 0) != strlen(message)) {
				perror("send failed");
			}
			printf("Welcome message sent successfully to %s\n", inet_ntoa(client.sin_addr));
		}
	}
}