// UDA_simulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8002"
//#define DEFAULT_PORT "8001"
#define MAX_MSG_LEN 256
#define MSG_SET_SENSORS 1
#define MSG_SET_EID     2

int main()
{
	char str[MAX_MSG_LEN];

	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	printf("Waiting for client connecting...\n");
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("accept from the client\n");
	}
#if 0
	//send multiple packages in one message to Beckhoff
								// 0xAA, 0, length, message id, ..., checksum, 0xEF
	char message_to_beckhoff[] = { (char)0xAA, 0,   6,      6,             12,        (char)0xEF,
		(char)0xAA, 0,  10,      2, 1, 50, 3, 5, 71,       (char)0xEF,
		(char)0xAA, 0,   6,      1,              7,        (char)0xEF,
		(char)0xAA, 0,   6,      6,             12,        (char)0xEF };

	for (;;)
	{
		send(ClientSocket, message_to_beckhoff, sizeof(message_to_beckhoff), 0);
		Sleep(100);
	}
#endif

	// No longer need server socket
	//closesocket(ListenSocket);

	// Send message to UDA
	while(1)
	{
#if 0
		printf("\n ******************************************************************************************\n");
		printf("Sending message to UDA(192.168.0.182) \n");
		printf("Please enter what you want send:\n");
		printf("For example: Sensor 11011 for 5s\n");
		printf("The order of the sensors are CowInBail Full  Half Trailing and Leading\n");
#endif
		scanf_s("%s", str, 100);

		char sensor[] = "sensor";
		char Sensor[] = "Sensor";
		unsigned char i = 0, j = 0, k = 0;
		char msg[23] = {0};
		unsigned char senbits = 0;
		int delaytime = 0;
		char t[50] = { 0 };

		for (; i < strlen(sensor); i++, j++)
		{
			if (str[i] != sensor[j] && str[i] != Sensor[j])
			{
				break;
			}
		}

		// set sensors
		if (j == strlen(sensor)) 
		{
			printf("Set sensors\n");
			j = 0;

			for (; i < strlen(str); i++)
			{
				if (j < 5)
				{
					if (' ' == str[i])
					{
						continue;
					}

					if ('1' == str[i] || '0' == str[i])
					{
						senbits = senbits << 1 | (str[i] - '0');
					}

					j++;
				}
				else
				{
					if ('0' <= str[i] && str[i] <= '9')
					{
						t[k] = str[i];
						k++;
					}
				}
			}

			delaytime = atoi(t);
			if (delaytime > 65535)
			{
				delaytime = 65535;
			}
#if 0
			msg[0] = (char)0xAA;
			msg[1] = 0;
			msg[2] = 9;
			msg[3] = MSG_SET_SENSORS;  
			msg[4] = senbits;
			msg[5] = (delaytime & 0xff00) >> 8;
			msg[6] = delaytime & 0xff;
			msg[7] = 0;  //checksum
			msg[8] = (char)0xEF;
#endif
			msg[0] = (char)0xAA;
			msg[1] = 0;
			msg[2] = '9';
			msg[3] = '8';
			msg[4] = '2';
			msg[5] = ' ';
			msg[6] = '0';
			msg[7] = '0';
			msg[8] = '0';
			msg[9] = '0';
			msg[10] = '0';
			msg[11] = '0';
			msg[12] = '0';
			msg[13] = '0';
			msg[14] = '0';
			msg[15] = '0';
			msg[16] = '0';
			msg[17] = '0';

			msg[18] = 0;
			msg[19] = 0;
			msg[20] = 255;
			msg[21] = 255;

			msg[22] = (char)0xEF;

			printf("sensor bits are %d, delaytime %d, msg[5] %d, msg[6] %d\n", senbits, delaytime, msg[5], msg[6]);

			//iSendResult = send(ClientSocket, msg, 9, 0);

			iSendResult = send(ClientSocket, msg, 23, 0);
			if (SOCKET_ERROR == iSendResult)
			{
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
		}
	}

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
    return 0;
}

