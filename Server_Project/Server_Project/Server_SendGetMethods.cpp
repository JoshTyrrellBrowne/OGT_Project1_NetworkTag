#include "Server.h"


bool Server::sendAll(int ID, char* data, int totalBytes)
{
	int bytesSent = 0; //Holds total bytes sent
	while (bytesSent < totalBytes) // While we still have more bytes to send
	{
		int RetnCheck = send(connections[ID].socket, data + bytesSent, totalBytes - bytesSent, NULL); //try to send remaining bytes
		if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to send
			return false; //Return false, failed to sendAll
		bytesSent += RetnCheck;
	}
	return true; //Success!
}

bool Server::recieveAll(int ID, char* data, int totalBytes)
{
	int bytesRecieved = 0; //Holds total bytes recieved
	while (bytesRecieved < totalBytes) // While we still have more bytes to recieve
	{
		int RetnCheck = recv(connections[ID].socket, data, totalBytes - bytesRecieved, NULL); //try to recieve remaining bytes
		if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to recieve
			return false; //Return false, failed to recieveAll
		bytesRecieved += RetnCheck; // Add to total bytes recieved
	}
	return true; //Success!
}

bool Server::SendInt(int ID, int t_int)
{
	t_int = htonl(t_int); //Convert long from Host Byte Order to Network Byte Order
	if (!sendAll(ID, (char*)&t_int, sizeof(int))) //If int failed to send due to connection issue 
		return false; //Return false: Connetion issue, int not successfully sent
	return true; //Return true: int successfully sent
}

bool Server::GetInt(int ID, int& t_int)
{
	if (!recieveAll(ID, (char*)&t_int, sizeof(int))) //Try to receive int, If there is a connection issue 
		return false; //Return false: did not recieve int
	t_int = ntohl(t_int); //Convert long from Network Byte Order to Host Byte Order
	return true; //Return true: int successfully retrieved
}

bool Server::SendPacketType(int ID, PacketType t_packetType)
{
	if (!SendInt(ID, (int)t_packetType)) //If packettype failed to send due to connection issue 
		return false; //Return false: Connetion issue, packettype not successfully sent
	return true; //Return true: packettype successfully sent
}

bool Server::GetPacketType(int ID, PacketType& t_packetType)
{
	int packetType;
	if (!GetInt(ID, packetType)) //Try to receive packettype, If there is a connection issue 
		return false; //Return false: did not recieve packettype
	t_packetType = (PacketType)packetType;
	return true; //Return true: packettype successfully retrieved
}

void Server::SendString(int ID, std::string& _string)
{
	PacketStructs::ChatMessage message(_string);
	connections[ID].packetManager.Append(message.toPacket());
}

bool Server::GetString(int ID, std::string& _string)
{
	int bufferLength; // Holds length of message
	if (!GetInt(ID, bufferLength)) //Get length of buffer and store it: bufferLength
		return false; //If get int fails, return false
	char* buffer = new char[bufferLength + 1]; //Allocate buffer
	buffer[bufferLength] = '\0'; // Set last char to null terminator so no junk

	if (!recieveAll(ID, buffer, bufferLength)) // recieve message and store in buffer
	{
		delete[] buffer; //delete buffer to prevent memory leak
		return false; //return false: Fails to recieve string buffer
	}

	_string = buffer; //set string to recieved buffer mesage
	delete[] buffer; //deallocate buffer memory (cleanup to prevent memory leak)
	return true; //Return true if we were successful in retrieving the string
}