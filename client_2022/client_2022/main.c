#pragma comment(lib,"ws2_32.lib")  // winsock2�� ����ϱ� ���� lib�� �߰��մϴ�.
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define DEFAULT_PORT "27015"
#define MY_LOCAL_IP "127.0.0.1"
//#define MY_LOCAL_IP "211.108.241.184"

// Client�� ���� ����
// 1. ���� ���� -> 2. ������ Connect -> 3. Send data -> 4. Recieve Reply

int main() {

    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in server;
    char server_reply[2000];
    int recv_size;
    char* message = malloc(sizeof(char) * 100);

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

    // 5. Receive reply from server
    if ((recv_size = recv(clientSocket, server_reply, 2000, 0)) == SOCKET_ERROR) {
        printf("Receive failed!\n");
    }
    else {
        printf("Welcome message from server : \n");
        server_reply[recv_size] = '\0';
        puts(server_reply);
    }

    // 4. Send data
    while (1) {
        //message
        printf("Enter your message(#exit to quit) : ");
        gets(message);
        if (strcmp(message, "#exit") == 0) {
            int iResult = shutdown(clientSocket, SD_SEND);
            if (iResult == SOCKET_ERROR) {
                printf("Shutdown failed : %d\n", WSAGetLastError());
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
            break;
        }
        if (send(clientSocket, message, strlen(message), 0) < 0) {
            printf("Send failed!\n");
            return 1;
        }
        else {
            printf("Message send : %s\n", message);
            //free(message);
        }

        // 5. Receive reply from server
        if ((recv_size = recv(clientSocket, server_reply, 2000, 0)) == SOCKET_ERROR) {
            printf("Receive failed!\n");
        }
        else {
            printf("Reply received : \n");
            server_reply[recv_size] = '\0';
            puts(server_reply);
        }
    }
    
    printf("Thank you, bye!(press any key)\n");
    getchar();
    //Sleep(5000);

    closesocket(clientSocket);
    WSACleanup();
}