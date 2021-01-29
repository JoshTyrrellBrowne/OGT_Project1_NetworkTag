#include "Client.h"

Client::Client(std::string IP, int PORT, Game& game)
{
	m_gamePtr = &game;
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) // If WSAStartup returns anything other than 0, means an error has occured in Winsock Startup
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	addr.sin_addr.s_addr = inet_addr(IP.c_str()); //Set IP to passed in IP
	//InetPton(AF_INET, _T("127.0.0.1"), &addr.sin_addr.s_addr); // URL explaining this: https://stackoverflow.com/questions/36683785/inet-addr-use-inet-pton-or-inetpton-instead-or-define-winsock-deprecated
	//addr.sin_addr.s_addr = htonl(INADDR_ANY); //Use this if u want to accept connections from any internet address
	addr.sin_port = htons(PORT); //Port 
	addr.sin_family = AF_INET; //IPv4 Socket
	clientptr = this; //update client ptr, used for calling methods inside static function
}

bool Client::Connect()
{
	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeOfAddr) != 0) //If we are unable to connect..
	{
		MessageBoxA(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	std::cout << "Connected!" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create client Thread 
	return true;
}

bool Client::CloseConnection()
{
	return false;
}

bool Client::ProcessPacket(PacketType packetType)
{
	switch (packetType)
	{
	case PacketType::ChatMessage:
	{
		std::string message; //string to store our message we recieved
		if (!GetString(message)) //Get chat message and store it in: message
			return false; //If we do not properly get the chat message, return false
		std::cout << message << std::endl; //Display the message to the user 
		break;
	}
	case PacketType::SetID:
	{
		int ID; //string to store our message we recieved
		if (!GetInt(ID)) //Get chat id and store it in: id
			return false; //If we do not properly get the id, return false
		m_gamePtr->setUpWithID(ID); //setID
		std::cout << "ID set!   ID: " << ID << std::endl; //Display the message to the user 
		break;
	}
	case PacketType::SetPosition:
	{
		int ID; //store id of player to set pos
		if (!GetInt(ID)) //Get id of player to move
			return false; //If we do not properly get the id, return false
		sf::Vector2f pos; // the position
		if (!GetPosition(pos)) //Get pos
			return false; //If we do not properly get the pos, return false
		m_gamePtr->setPosition(ID, pos);
		break;
	}
	case PacketType::TagPlayer:
		int ID; //store id of player to tag
		if (!GetInt(ID)) //Get id of player to tag
			return false; //If we do not properly get the id, return false
		m_gamePtr->tagPlayer(ID);
		break;
	case PacketType::Reset:
		m_gamePtr->reset();
		break;
	default:
		std::cout << "Unrecognised packet: " << (int)packetType << std::endl;
		break;
	}
	return true;
}

void Client::ClientThread()
{
	PacketType packetType;
	while (true)
	{
		//First get the packet type
		if (!clientptr->GetPacketType(packetType)) //get packet type
			break; //If there is an issue getting the packet type, exit loop
		if (!clientptr->ProcessPacket(packetType)) //if failed to process packet, break out
			break; //break out of our client handler loop
	}
	std::cout << "Lost connection to the server." << std::endl;
	if (clientptr->CloseConnection())
	{
		std::cout << "Socket to the server was closed successfully." << std::endl;
	}
	else
	{
		std::cout << "Socket was not able to be closed." << std::endl;
	}
}
