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

HANDLE hMutex; // ���ؽ� ����

int ret = 0;
SOCKET clientSocket = 0;
// Ŭ���̾�Ʈ �ʱ�ȭ �� ���� ����
int client_init() {
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

	// 3. Connect to server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
		printf("Connect error!\n");
		return 1;
	}
	else {
		printf("Connection has been completed!\n");
	}

	return s;
}

// �޽��� �޴� ������ �Լ�
void recv_thread(void* pData) {
	int ret_thread = 65535;
	char buff_thread[1024] = { 0 };

	// ������� ���� ���� ���ϴ� ���� �ƴϸ� �޴� �߿� ������ ����� ���� ��
	while (ret_thread != INVALID_SOCKET || ret_thread != SOCKET_ERROR) {
		Sleep(10);

		// �������� �ִ� �޽����� �ǽð����� ��ٷȴٰ� �޴´�.
		ret_thread = recv(clientSocket, buff_thread, sizeof(buff_thread), 0);

		// �������� �޴� �۾��� �� ��� �������̸� Ż��
		if (ret_thread == INVALID_SOCKET || ret_thread == SOCKET_ERROR) {
			break;
		}

		// ���������� �޴´ٸ� ���� ���� ���
		printf("\nmessage recieve : %s", buff_thread);
		memset(buff_thread, 0, 1024); // ���� �ʱ�ȭ
	}

	// �۾� ���� ���� �ʱ�ȭ
	WaitForSingleObject(hMutex, 100L);
	ret = INVALID_SOCKET;
	ReleaseMutex(hMutex);
	return;
}

int main() {
	// ���ؽ� �ʱ�ȭ
	struct sockaddr_in client;
	hMutex = CreateMutex(NULL, FALSE, NULL);
	char buff[1024];
	int recv_size;

	if (!hMutex) {
		printf("Mutex create error\n");
		return 1;
	}

	clientSocket = client_init();
	if (clientSocket == SOCKET_ERROR) {
		printf("socket init error\n");
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	// 3.5 Receive welcome message from server
	if ((ret = recv(clientSocket, buff, 1024, 0)) == SOCKET_ERROR) {
		printf("Receive failed!\n");
	}
	else {
		printf("Welcome message from server : \n");
		buff[ret] = '\0';
		puts(buff);
	}

	if (!strcmp("Client Full!\n", buff)) {
		closesocket(clientSocket);
		WSACleanup();
		return 0;
	}

	// ���� ������ �Ǹ� ������ �۵� - �޴� �޽��� ������ �ǽð� ����
	_beginthread(recv_thread, 0, NULL);

	// ������ �޽����� ������� ���� �ʿ䰡 ����
	// ���� ��� �߸��� ���� ������ Ż��
	while (ret != INVALID_SOCKET || ret != SOCKET_ERROR) {
		Sleep(10);
		printf("���� �޽��� �Է� : ");
		fgets(buff, 1024, stdin);
		if (strcmp(buff, "#exit") == 0) {
			break;
		}

		// ������ ������ ���� fgets�� �ް� ���� ���������� ����
		ret = send(clientSocket, buff, strlen(buff), 0);
		
		memset(buff, 0, 1024);
	}
	printf("������ ������ ���������ϴ�(PRESS ANY KEY)\n");
	closesocket(clientSocket);
	WSACleanup();

	getchar();
}