// Server
// Author: Josh Tyrrell Browne

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include <iostream>

SOCKET Connections[100];
int ConnectionCounter = 0;

enum Packet
{
	P_ChatMessage
};

bool SendInt(int ID, int t_int)
{
	int RetnCheck = send(Connections[ID], (char*)&t_int, sizeof(int), NULL); //send int
	if (RetnCheck == SOCKET_ERROR) //If int failed to send due to connection issue 
		return false; //Return false: Connetion issue
	return true; //Return true: int successfully sent
}

bool GetInt(int ID, int& t_int)
{
	int RetnCheck = recv(Connections[ID], (char*)&t_int, sizeof(int), NULL); //recieve int
	if (RetnCheck == SOCKET_ERROR) //If there is a connection issue 
		return false; //Return false: did not recieve int
	return true; //Return true: int successfully retrieved
}

bool SendPacketType(int ID, Packet t_packetType)
{
	int RetnCheck = send(Connections[ID], (char*)&t_packetType, sizeof(Packet), NULL); //send packet
	if (RetnCheck == SOCKET_ERROR) //If packet failed to send due to connection issue 
		return false; //Return false: Connetion issue
	return true; //Return true: packet successfully sent
}

bool GetPacketType(int ID, Packet& t_packetType)
{
	int RetnCheck = recv(Connections[ID], (char*)&t_packetType, sizeof(Packet), NULL); //recieve packet
	if (RetnCheck == SOCKET_ERROR) //If there is a connection issue 
		return false; //Return false: did not recieve packet
	return true; //Return true: packet successfully retrieved
}

bool SendString(int ID, std::string& _string)
{
	if (!SendPacketType(ID, P_ChatMessage)) //Send Packet type: Chat Message
		return false; //Return false: Failed to send string
	int bufferLength = _string.size(); //find string buffer length
	if (!SendInt(ID, bufferLength)) //Send length of string, if sending buffer length failed,
		return false; //failed to send string buffer length, return false
	int RetnCheck = send(Connections[ID], _string.c_str(), bufferLength, NULL); //send string
	if (RetnCheck == SOCKET_ERROR) //If packet failed to send due to connection issue 
		return false; //Return false: Connetion issue
	return true; //Return true: packet successfully sent
}

bool GetString(int ID, std::string& _string)
{
	int bufferLength; // Holds length of message
	if (!GetInt(ID, bufferLength)) //Get length of buffer and store it: bufferLength
		return false; //If get int fails, return false
	char* buffer = new char[bufferLength + 1]; //Allocate buffer
	buffer[bufferLength] = '\0'; // Set last char to null terminator so no junk
	int RetnCheck = recv(Connections[ID], buffer, bufferLength, NULL); //receive message and store
	_string = buffer; //set string to recieved buffer mesage
	delete[] buffer; //deallocate buffer memory (cleanup to prevent memory leak)
	if (RetnCheck == SOCKET_ERROR) //If connection is lost while getting message
		return false; //If there is an issue with connection, return false
	return true; //Return true if we were successful in retrieving the string
}

bool ProcessPacket(int ID, Packet packetType)
{
	switch (packetType)
	{
	case P_ChatMessage:
	{
		std::string message; //string to store our message we recieved
		if (!GetString(ID, message)) //Get the chat message and store it in variable
			return false; //If we dont properly get the chat message, return false

		for (int i = 0; i < ConnectionCounter; i++) // Next we need to send message to each user
		{
			if (i == ID) //If connection is user who sent the message
				continue; //Skip to the next user, don't send message back to whom sent it
			if (!SendString(i, message)) //Send message to connection at index i, if it fails to be sent...
			{
				std::cout << "Failed to send message from client ID: " << ID << " to client ID: " << i << std::endl;
			}
		}
		std::cout << "Processed chat message packet from user ID: " << ID << std::endl;
		break;
	}
	default:
		std::cout << "Unrecognised packet: " << packetType << std::endl;
		break;
	}
	return true;
}

void ClientHandlerThread(int ID) //ID = the index in the SOCKET connetions array
{
	Packet packetType;
	while (true)
	{
		if (!GetPacketType(ID, packetType)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
		if (!ProcessPacket(ID, packetType)) //if packet not properly processed
			break; // break out 
	}
	std::cout << "Lost connection to client ID: " << ID << std::endl;
	closesocket(Connections[ID]); //close the socket that was being used for client connection
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

	// Setup socket address
	SOCKADDR_IN addr; // Address tthat we will bind our listening socket to
	int addrlen = sizeof(addr); //length of address (required for accept call)
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Brodcast locally, Address = localhost (this pc)
	InetPton(AF_INET, _T("127.0.0.1"), &addr.sin_addr.s_addr);
	//addr.sin_addr.s_addr = htonl(INADDR_ANY); //Use this if u want to accept connections from any internet address
	addr.sin_port = htons(1111); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	//Make socket listen
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listien for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //Bind the adrress to the socket
	listen(sListen, SOMAXCONN); //Places sListen socket in a state in which it is listening for an incoming connection, **Note: SOMAXCONN= socket outstanding max connections(total amount of people that can try and connect at once)

	SOCKET newConnection; //Socket to hold the client's connection
	for (int i = 0; i < 100; i++) { //holds up to 100 connections
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
		if (newConnection == 0) //If accepting the client connection failed
		{
			std::cout << "Failed to accept the client's connection" << std::endl;
		}
		else // if client conection properly accepted
		{
			std::cout << "Client Connected!" << std::endl;
			Connections[i] = newConnection;
			ConnectionCounter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
			std::string MOTD = "Welcome! This is the Message of the Day."; // Create buffer with message of the day
			SendString(i, MOTD);
		}
	}



	system("pause");
	return 0;
}