#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <string.h>

#define PORT 80

const char szHost[]{ "www.google.com" };


int main(const int argc, const char *argv[]) {

	// Init WINSOCK
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		fprintf(stderr, "WSAStartup()\n");
		getchar();
		ExitProcess(EXIT_FAILURE);
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		fprintf(stderr, "socket()\n");
		getchar();
		ExitProcess(EXIT_FAILURE);
	}

	// Get server info
	HOSTENT *host = gethostbyname(szHost);
	if (host == nullptr) {
		closesocket(sock);
		fprintf(stderr, "gethostbyname()\n");
		getchar();
		ExitProcess(EXIT_FAILURE);
	}

	// Define server info
	SOCKADDR_IN sin;
	ZeroMemory(&sin, sizeof(sin));
	sin.sin_port = htons(PORT);
	sin.sin_family = AF_INET;
	memcpy(&sin.sin_addr.S_un.S_addr, host->h_addr_list[0], sizeof(sin.sin_addr.S_un.S_addr));

	// Connect to server
	if (connect(sock, (const sockaddr *)&sin, sizeof(sin)) != 0) {
		closesocket(sock);
		fprintf(stderr, "connect()\n");
		getchar();
		ExitProcess(EXIT_FAILURE);
	}

	// Send info to server
	const char szMsg[] = "HEAD / HTTP/1.0\r\n\r\n";
	if (!send(sock, szMsg, strlen(szMsg), 0)) {
		closesocket(sock);
		fprintf(stderr, "send()\n");
		getchar();
		ExitProcess(EXIT_FAILURE);
	}


	char szBuffer[4096];
	char szTemp[4096];
	while (recv(sock, szTemp, 4096, 0))
	{
		strcat(szBuffer, szTemp);
	}

	printf("%s\n", szBuffer);

	closesocket(sock);
	getchar();

	ExitProcess(EXIT_SUCCESS);
}