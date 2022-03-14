#pragma comment(lib,"ws2_32.lib")  // winsock2�� ����ϱ� ���� lib�� �߰��մϴ�.
#include <stdio.h>
#include <WinSock2.h>

// ������ ������ ���� �⺻���� �帧
// Winsock �ʱ�ȭ -> socket ���� -> network ��� -> socket �ݱ� -> Winsock ����

int main() {

	//WSADATA ����ü���� Windows ���� ������ ���� ������ ���ԵǾ� ����
	WSADATA wsaData;
	SOCKET serverSocket;

	// 1. Winsock �ʱ�ȭ, WINSOCK VERSION 2.2
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed: %d\n", WSAStartup(MAKEWORD(2, 2), &wsaData));
		return 1;
	}
	else {
		printf("WSAStartup completed!!\n");
	}

	// 2. �ʱ�ȭ �� �������� ����� SOCKET �ν��Ͻ� ����
	// socket() function is used to create a socket
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d\n", WSAGetLastError());
	}
	else {
		printf("Socket has been created!\n");
		// Address Family : AF_INET ( = IPv4)
		// Type : SOCK_STREAM( = TCP Protocol connection)
		// Protocol : 0
	}
}