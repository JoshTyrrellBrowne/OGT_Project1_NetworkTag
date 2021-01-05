// Server
// Author: Josh Tyrrell Browne
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include <iostream>
#include <string>

enum Packet
{
	P_ChatMessage
};

class Server
{
public:
	Server(int PORT, bool BroadcastPublically = false);
	bool ListenForNewConnection();

private:
	bool SendInt(int ID, int t_int);
	bool GetInt(int ID, int& t_int);

	bool SendPacketType(int ID, Packet t_packetType);
	bool GetPacketType(int ID, Packet& t_packetType);

	bool SendString(int ID, std::string& _string);
	bool GetString(int ID, std::string& _string);

	bool ProcessPacket(int ID, Packet packetType);

	static void ClientHandlerThread(int ID);

private:
	SOCKET Connections[100];
	int ConnectionCounter = 0;

	SOCKADDR_IN addr; // Address tthat we will bind our listening socket to
	int addrlen = sizeof(addr); //length of address (required for accept call)
	SOCKET sListen;
};

static Server * serverPtr; //This ptr is needed so the static ClientHandler method can access the server methods