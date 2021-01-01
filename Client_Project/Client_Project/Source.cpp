//Client 
// Author: Josh Tyrrell Browne

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include <string>
#include <iostream>

SOCKET Connection; // connection socket

enum Packet
{
	P_ChatMessage
};


bool ProcessPacket(Packet packetType)
{
	switch (packetType)
	{
	case P_ChatMessage:
	{
		int bufferLength; // Holds length of buffer
		recv(Connection, (char*)&bufferLength, sizeof(int), NULL);//Recieve buffer length
		char* buffer = new char[bufferLength + 1]; //Allocate buffer
		buffer[bufferLength] = '\0'; //Set last character of buffer to be null terminator so we arent printing info we dont want
		recv(Connection, buffer, bufferLength, NULL);//Recieve buffer message from server
		std::cout << buffer << std::endl;
		delete[] buffer; //Deallocate buffer
		break;
	}
	default:
		std::cout << "Unrecognised packet: " << packetType << std::endl;
		break;
	}
	return true;
}

void ClientThread()
{
	Packet packetType;
	while (true)
	{
		recv(Connection, (char*)&packetType, sizeof(Packet), NULL);//Recieve packet type

		if (!ProcessPacket(packetType)) //if failed to process packet, break out
			break;
	}
	closesocket(Connection); //close the socket that was being used for client connection
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


	std::string buffer; // string buffer to send message
	while (true)
	{
		std::getline(std::cin, buffer); //Get line if user presses enter and fill buffer
		int bufferLength = buffer.length();
		Packet packetType = P_ChatMessage; //Create packet type: Chat message to be sent to server
		send(Connection, (char*)&packetType, sizeof(Packet), NULL);// Send packet type
		send(Connection, (char*)&bufferLength, sizeof(int), NULL); //Send length of buffer (message)
		send(Connection, buffer.c_str(), bufferLength, NULL); //Send actual message (buffer)
		Sleep(10);
	}

	system("pause");
	return 0;
}