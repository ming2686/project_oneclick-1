#pragma comment(lib,"ws2_32.lib")  // winsock2�� ����ϱ� ���� lib�� �߰��մϴ�.
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>
#include <io.h>
#define DEFAULT_PORT "27015"
#define CLIENT_SOCKET_NUMBER 30
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

// ������ ������ ���� �⺻���� �帧
// Winsock �ʱ�ȭ -> socket ���� -> network ��� -> socket �ݱ� -> Winsock ����

int main() {

	//WSADATA ����ü���� Windows ���� ������ ���� ������ ���ԵǾ� ����
	WSADATA wsaData;
	SOCKET serverSocket;
	SOCKET newSocket;
	SOCKET s;
	SOCKET t;
	SOCKET master, client_socket[CLIENT_SOCKET_NUMBER]; // master ������� ���� ���� serverSocket ���
	struct sockaddr_in server;
	struct sockaddr_in client;
	struct sockaddr_in address;
	char *message = NULL;
	int c;
	int max_clients = CLIENT_SOCKET_NUMBER, activity, addrlen, i, valread;
	int MAXRECV = 1024; // Recieve ���� �ִ� ũ��
	fd_set readfds; // Set of socket descriptors
	char* buffer = malloc(sizeof(char) * (MAXRECV + 1)); // NULL character�� ���� 1���� �߰� ����

	// Client socket �迭 �ʱ�ȭ
	for (int i = 0; i < 30; i++) {
		client_socket[i] = 0;
	}

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
	server.sin_addr.s_addr = inet_addr("211.215.249.35");
	server.sin_family = AF_INET; // IPv4
	server.sin_port = htons(9090); // ����� ��Ʈ ��ȣ ����

	// 3. Socket ���� �� Bind
	if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code : %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
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

	addrlen = sizeof(struct sockaddr_in);

	// 5. Client�� ���� ��û�� accept�ϱ�
	while (TRUE) {
		// clear the socket fd set
		FD_ZERO(&readfds);

		// add server socket to fd set
		FD_SET(serverSocket, &readfds);

		// add child sockets to fd set
		for (int i = 0; i < max_clients; i++) {
			 s = client_socket[i];
			 if (s > 0) {
				 FD_SET(s, &readfds);
			 }
		}

		// wait for an activity on any of the sockets, timeout = NULL -> ���Ѵ�� ��ٸ�
		activity = select(0, &readfds, NULL, NULL, NULL);


		if (activity == SOCKET_ERROR) {
			perror("accept error");
			exit(EXIT_FAILURE);
		}

		// ���� server socket���� ���� ���� �Ͼ�� -> ���� ��û�� ������ ��
		if (FD_ISSET(serverSocket, &readfds)) {
			if ((newSocket = accept(serverSocket, (struct sockaddr*)&address, (int*)&addrlen)) < 0) {
				perror("accept");
				exit(EXIT_FAILURE);
			}
			printf("New connection, socket fd is %d, ip is : %s, port : %d\n", newSocket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));


			// 6. ���� ��û�� ���� client�� welcome message send�ϱ�
			message = "Welcome client!!\n";
			if (send(newSocket, message, strlen(message), 0) != strlen(message)) {
				perror("send failed");
			}
			printf("Welcome message sent successfully to %s\n", inet_ntoa(address.sin_addr));

			// add new socket to array of sockets
			for (int i = 0; i < max_clients; i++) {
				if (client_socket[i] == 0) {
					client_socket[i] = newSocket;
					printf("Adding to list of sockets at index %d\n", i);
					break;
				}
			}
		}
		// �ٸ� socket�� ���� IO OPERATION
		else {
			for (int i = 0; i < max_clients; i++) {
				s = client_socket[i];
				// if client presend in read socket
				if (FD_ISSET(s, &readfds)) {
					// get details of the client
					getpeername(s, (struct sockaddr* )&address, (int*)&addrlen);

					// 7. client�κ��� ���� ������ �о���̱�
					valread = recv(s, buffer, MAXRECV, 0);

					if (valread == SOCKET_ERROR) {
						int error_code = WSAGetLastError();
						if (error_code == WSAECONNRESET) {
							// � client�� ������ ������������ ����, ������ ǥ���Ѵ�
							printf("Host disconnected unexpectedly, ip : %s, port %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

							// �ش� ���� close �� 0���� ǥ��(������ ����)
							closesocket(s);
							client_socket[i] = 0;
						}
					}
					if (valread == 0) {
						// client�� ���������� ���� ����, ���� ǥ��
						printf("Host disconnected, ip : %s, port %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

						// �ش� ���� close �� 0���� ǥ��(������ ����)
						closesocket(s);
						client_socket[i] = 0;
					}

					// Echo back the message that came in
					else {
						// add null character, ���ڿ��� �ٷ�� ���� �۾�
						buffer[valread] = '\0';
						printf("%s:%d - %s\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port), buffer);
						/*for (int i = 0; i < max_clients; i++) {
							if (client_socket != 0) {
								send(client_socket[i], buffer, valread, 0);
								printf("client_socket[%d] = %d\n", i, client_socket[i]);
							}
						}*/
						//send(s, buffer, valread, 0);
						send(client_socket[1], buffer, valread, 0);
					}
				}
			}
		}
	}


	closesocket(newSocket);
	closesocket(serverSocket);
	WSACleanup();

}