#pragma once
//Client 
// Author: Josh Tyrrell Browne

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <SFML/Graphics.hpp>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include "PacketType.h"

class Game;

class Client
{
public:
	Client(std::string IP, int PORT, Game& game);
	bool Connect();
	bool CloseConnection();
	bool SendString(std::string& string);

	bool ProcessPacket(PacketType packetType);
	static void ClientThread();

	//Sending Funcs
	bool sendAll(char* data, int totalbytes);
	bool SendInt(int t_int);
	bool SendPacketType(PacketType packetType);
	bool SendTagPlayer(int t_idToTag);

	//Getting Functions
	bool recieveAll(char* data, int totalbytes);
	bool GetInt(int& t_int);
	bool GetPacketType(PacketType& packetType);
	bool GetString(std::string& string);
	bool GetPosition(sf::Vector2f& t_pos);

private:
	SOCKET Connection; // connection socket
	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeOfAddr = sizeof(addr); //Need size of addr for connect function
	
	Game* m_gamePtr;
};

static Client* clientptr;
#include "Game.h"