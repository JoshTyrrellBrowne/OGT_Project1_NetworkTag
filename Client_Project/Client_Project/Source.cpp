//Client 
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include <string>
#include <iostream>

SOCKET Connection; // connection socket

void ClientThread()
{
	char buffer[256];
	while (true)
	{
		recv(Connection, buffer, sizeof(buffer), NULL);
		std::cout << buffer << std::endl;
	}
}

int main()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) // If WSAStartup returns anything other than 0, means an error has occured in Winsock Startup
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}


	SOCKADDR_IN addr; // Address tthat we will bind our connection socket to
	int addrlen = sizeof(addr); //length of address (required for connect call)
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Brodcast locally, Address = localhost (this pc)
	InetPton(AF_INET, _T("127.0.0.1"), &addr.sin_addr.s_addr); // URL explaining this: https://stackoverflow.com/questions/36683785/inet-addr-use-inet-pton-or-inetpton-instead-or-define-winsock-deprecated
	//addr.sin_addr.s_addr = htonl(INADDR_ANY); //Use this if u want to accept connections from any internet address
	addr.sin_port = htons(1111); //Port 1111
	addr.sin_family = AF_INET; //IPv4 Socket


	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, addrlen) != 0) //If we are unable to connect..
	{
		MessageBoxA(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
		return 0; //Failed to Connect
	}
	std::cout << "Connected!" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create client Thread 


	char buffer[256];
	while (true)
	{
		std::cin.getline(buffer, sizeof(buffer));
		send(Connection, buffer, sizeof(buffer), NULL);
		Sleep(10);
	}

	system("pause");
	return 0;
}