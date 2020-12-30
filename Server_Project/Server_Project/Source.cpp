// Server
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include <iostream>

SOCKET Connections[100];
int ConnectionCounter = 0;

void ClientHandlerThread(int index)
{
	int bufferLength; // Holds length of buffer
	while (true)
	{
		recv(Connections[index], (char*)&bufferLength, sizeof(int), NULL); // get buffer length
		char* buffer = new char[bufferLength]; //Buffer to hold recieved message
		recv(Connections[index], buffer, bufferLength, NULL); // Recieve message from client


		for (int i = 0; i < ConnectionCounter; i++)
		{
			if (i == index) // skip user who sent msg
				continue;
			send(Connections[i], (char*)&bufferLength, sizeof(int), NULL);// Send buffer length to client
			send(Connections[i], buffer, bufferLength, NULL);// Send buffer message to same client
		}
		delete[] buffer;
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
			std::string MOTD = "Welcome! This is the Message of the Day."; // Create buffer with message of the day
			int MOTDLength = MOTD.size(); //Length of message
			send(newConnection, (char*)&MOTDLength, sizeof(int), NULL); //Send MOTD buffer length
			send(newConnection, MOTD.c_str(), MOTDLength, NULL); //Send MOTD buffer
			Connections[i] = newConnection;
			ConnectionCounter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
		}
	}



	system("pause");
	return 0;
}