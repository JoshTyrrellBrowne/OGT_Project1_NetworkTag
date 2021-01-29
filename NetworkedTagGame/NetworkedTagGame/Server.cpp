#include "Server.h"

Server::Server(int PORT, bool BroadcastPublically) //Port = port to broadcast on. BP is to broadcast to 
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
	if (BroadcastPublically) //If server is open to public
		addr.sin_addr.s_addr = htonl(INADDR_ANY); //Use this if u want to accept connections from any internet address 
	else //If server is only for our router
		addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Brodcast locally, Address = localhost (this pc)
		//InetPton(AF_INET, _T("127.0.0.1"), &addr.sin_addr.s_addr);
	
	addr.sin_port = htons(PORT); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	//Make socket listen
	sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listien for new connections
	
	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) //Bind the adrress to the socket
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) //Places sListen socket in a state in which it is listening for an incoming connection, **Note: SOMAXCONN= socket outstanding max connections(total amount of people that can try and connect at once)
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	serverPtr = this;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PacketSenderThread, NULL, NULL, NULL); //Create thread that will manage outgoing packets
	//std::thread listenForNewConnectionsThread(ListenForNewConnection, std::ref(*this));
//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ListenForNewConnectionThread, NULL, NULL, NULL);
}

bool Server::ListenForNewConnection()
{
	SOCKET newConnection; //Socket to hold the client's connection
	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
	if (newConnection == 0) //If accepting the client connection failed
	{
		std::cout << "Failed to accept the client's connection" << std::endl;
		return false;
	}
	else // if client conection properly accepted
	{
		std::cout << "Client Connected! ID:" << ConnectionCounter << std::endl;
		connections[ConnectionCounter].socket = newConnection;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(ConnectionCounter), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
		std::string MOTD = "Welcome! This is the Message of the Day."; // Create buffer with message of the day
		SendString(ConnectionCounter, MOTD);
		SendID(ConnectionCounter, ConnectionCounter); //Send this player its ID 
		ConnectionCounter++;
		return true;
	}

}
//
//void Server::ListenForNewConnectionThread()
//{
//	while (true)
//	{
//		for (int i = 0; i < 100; i++)
//		{
//			serverPtr->ListenForNewConnection();
//		}
//	}
//}

//void Server::ListenForNewConnection(Server& t_server)
//{
//	while (true)
//	{
//		//SOCKET newConnection; //Socket to hold the client's connection
//		//serverPtr->GetInt(1, i);
//		//serverPtr->connections[0] = new Connection();
//		//serverPtr->newConnection = accept(s->sListen, NULL, NULL);
//		int j;
//		j = serverPtr->i;
//		t_server.newConnection = accept(t_server.sListen, (SOCKADDR*)&(t_server.addr), &(t_server.addrlen)); //Accept a new connection
//		if (serverPtr->newConnection == 0) //If accepting the client connection failed
//		{
//			std::cout << "Failed to accept the client's connection" << std::endl;
//		}
//		else
//		{
//			std::cout << "Client Connected! ID:" << serverPtr->ConnectionCounter << std::endl;
//			serverPtr->connections[serverPtr->ConnectionCounter].socket = serverPtr->newConnection;
//			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(serverPtr->ConnectionCounter), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
//			std::string MOTD = "Welcome! This is the Message of the Day."; // Create buffer with message of the day
//			serverPtr->SendString(serverPtr->ConnectionCounter, MOTD);
//			serverPtr->SendID(serverPtr->ConnectionCounter, serverPtr->ConnectionCounter); //Send this player its ID
//			serverPtr->ConnectionCounter++;
//			serverPtr->newConnection = 0;
//		}
//	}
//}


bool Server::ProcessPacket(int ID, PacketType packetType)
{
	switch (packetType)
	{
	case PacketType::ChatMessage:
	{
		std::string message; //string to store our message we recieved
		if (!GetString(ID, message)) //Get the chat message and store it in variable
			return false; //If we dont properly get the chat message, return false

		for (int i = 0; i < ConnectionCounter; i++) // Next we need to send message to each user
		{
			if (i == ID) //If connection is user who sent the message
				continue; //Skip to the next user, don't send message back to whom sent it
			SendString(i, message); //send message to connection i
		}
		std::cout << "Processed chat message packet from user ID: " << ID << std::endl;
		break;
	}
	case PacketType::SetPosition:
	{
		int playerID; //store id of player to set pos
		if (!GetInt(ID, playerID)) //Get chat id and store it in: id///////////////////////////////////////////////////////////
			return false; //If we do not properly get the id, return false
		float xPos;
		float yPos;
		if (!GetPosition(ID, xPos, yPos)) //Get chat id and store it in: id
			return false; //If we do not properly get the id, return false
		for (int i = 0; i < ConnectionCounter; i++) // Next we need to send message to each user
		{
			if (i == ID) //If connection is user who sent the message
				continue; //Skip to the next user, don't send message back to whom sent it
			//SendPacketType()
			SendPosition(i,ID, xPos, yPos); //send message to connection i
		}
		std::cout << "Processed setPosition from user ID: " << ID << std::endl;
		break;
	}
	default:
		std::cout << "Unrecognised packet: " << (int)packetType << std::endl;
		break;
	}
	return true;
}

void Server::ClientHandlerThread(int ID) //ID = the index in the SOCKET connetions array
{
	PacketType packetType;
	while (true)
	{
		if (!serverPtr->GetPacketType(ID, packetType)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
		if (!serverPtr->ProcessPacket(ID, packetType)) //if packet not properly processed
			break; // break out 
	}
	std::cout << "Lost connection to client ID: " << ID << std::endl;
	closesocket(serverPtr->connections[ID].socket); //close the socket that was being used for client connection
}

void Server::PacketSenderThread() //Thread for outgoing packets
{
	while (true)
	{
		for (int i = 0; i < serverPtr->ConnectionCounter; i++) //for each connection
		{
			if (serverPtr->connections[i].packetManager.HasPendingPackets())
			{
				Packet p = serverPtr->connections[i].packetManager.Retrieve(); //Retrieve a packet
				if (!serverPtr->sendAll(i, p.buffer, p.size)) //send packet
				{
					std::cout << "Failed to send packet to ID: " << i << std::endl;
				}
				delete p.buffer;
			}
		}
		//Sleep(5);
	}
}
