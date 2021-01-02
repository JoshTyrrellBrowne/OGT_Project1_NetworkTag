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

bool SendInt(int t_int)
{
	int RetnCheck = send(Connection, (char*)&t_int, sizeof(int), NULL); //send int
	if (RetnCheck == SOCKET_ERROR) //If int failed to send due to connection issue 
		return false; //Return false: Connetion issue
	return true; //Return true: int successfully sent
}

bool GetInt(int& t_int)
{
	int RetnCheck = recv(Connection, (char*)&t_int, sizeof(int), NULL); //recieve int
	if (RetnCheck == SOCKET_ERROR) //If there is a connection issue 
		return false; //Return false: did not recieve int
	return true; //Return true: int successfully retrieved
}

bool SendPacketType(Packet t_packetType)
{
	int RetnCheck = send(Connection, (char*)&t_packetType, sizeof(int), NULL); //send packet
	if (RetnCheck == SOCKET_ERROR) //If packet failed to send due to connection issue 
		return false; //Return false: Connetion issue
	return true; //Return true: packet successfully sent
}

bool GetPacketType(Packet & t_packetType)
{
	int RetnCheck = recv(Connection, (char*)&t_packetType, sizeof(int), NULL); //recieve packet
	if (RetnCheck == SOCKET_ERROR) //If there is a connection issue 
		return false; //Return false: did not recieve packet
	return true; //Return true: packet successfully retrieved
}

bool SendString(std::string& _string)
{
	if (!SendPacketType(P_ChatMessage)) //Send Packet type: Chat Message
		return false; //Return false: Failed to send string
	int bufferLength = _string.size(); //find string buffer length
	if (!SendInt(bufferLength)) //Send length of string, if sending buffer length failed,
		return false; //failed to send string buffer length, return false
	int RetnCheck = send(Connection, _string.c_str(), bufferLength, NULL); //send string
	if (RetnCheck == SOCKET_ERROR) //If packet failed to send due to connection issue 
		return false; //Return false: Connetion issue
	return true; //Return true: packet successfully sent
}

bool GetString(std::string& _string)
{
	int bufferLength; // Holds length of message
	if (!GetInt(bufferLength)) //Get length of buffer and store it: bufferLength
		return false; //If get int fails, return false
	char* buffer = new char[bufferLength + 1]; //Allocate buffer
	buffer[bufferLength] = '\0'; // Set last char to null terminator so no junk
	int RetnCheck = recv(Connection, buffer, bufferLength, NULL); //receive message and store
	_string = buffer; //set string to recieved buffer mesage
	delete[] buffer; //deallocate buffer memory (cleanup to prevent memory leak)
	if (RetnCheck == SOCKET_ERROR) //If connection is lost while getting message
		return false; //If there is an issue with connection, return false
	return true; //Return true if we were successful in retrieving the string
}


bool ProcessPacket(Packet packetType)
{
	switch (packetType)
	{ 
	case P_ChatMessage:
	{
		std::string message; //string to store our message we recieved
		if (!GetString(message)) //Get chat message and store it in: message
			return false; //If we do not properly get the chat message, return false
		std::cout << message << std::endl; //Display the message to the user 
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
		//First get the packet type
		if (!GetPacketType(packetType)) //get packet type
			break; //If there is an issue getting the packet type, exit loop
		if (!ProcessPacket(packetType)) //if failed to process packet, break out
			break; //break out of our client handler loop
	}
	std::cout << "Lost connection to the server." << std::endl;
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


	std::string userInput; // string buffer to send message
	while (true)
	{
		std::getline(std::cin, userInput); //Get line if user presses enter and fill buffer
		if (!SendString(userInput)) //send string: userInput
			break;
		Sleep(10);
	}

	system("pause");
	return 0;
}