#pragma comment(lib,"ws2_32.lib")  // winsock2�� ����ϱ� ���� lib�� �߰��մϴ�.
#include <stdio.h>
#include <WinSock2.h>

// ������ ������ ���� �⺻���� �帧
// Winsock �ʱ�ȭ -> socket ���� -> network ��� -> socket �ݱ� -> Winsock ����

int main() {

	//WSADATA ����ü���� Windows ���� ������ ���� ������ ���ԵǾ� ����
	WSADATA wsaData;

	// 1. Winsock �ʱ�ȭ, WINSOCK VERSION 2.2
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed: %d\n", WSAStartup(MAKEWORD(2, 2), &wsaData));
		return 1;
	}
	else {
		printf("WSAStartup completed!!\n");
	}

	// 2. �ʱ�ȭ �� �������� ����� SOCKET �ν��Ͻ� ����

}