#pragma once
//Client 
// Author: Josh Tyrrell Browne

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include <string>
#include <iostream>


enum Packet
{
	P_ChatMessage
};

class Client
{
public:
	Client(std::string IP, int PORT);
	bool Connect();
	bool CloseConnection();
	bool SendString(std::string& string);
private:
	bool ProcessPacket(Packet packetType);
	static void ClientThread();

	//Sending Funcs
	bool sendAll(char* data, int totalbytes);
	bool SendInt(int t_int);
	bool SendPacketType(Packet packetType);

	//Getting Functions
	bool recieveAll(char* data, int totalbytes);
	bool GetInt(int& t_int);
	bool GetPacketType(Packet& packetType);
	bool GetString(std::string& string);

private:
	SOCKET Connection; // connection socket
	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeOfAddr = sizeof(addr); //Need size of addr for connect function
};

static Client* clientptr;