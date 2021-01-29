#include "Client.h"

bool Client::sendAll(char* data, int totalBytes)
{
	int bytesSent = 0; //Holds total bytes sent
	while (bytesSent < totalBytes) // While we still have more bytes to send
	{
		int RetnCheck = send(Connection, data + bytesSent, totalBytes - bytesSent, NULL); //try to send remaining bytes
		if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to send
			return false; //Return false, failed to sendAll
		bytesSent += RetnCheck;
	} 

	return true; //Success!
}

bool Client::SendInt(int t_int)
{
	t_int = htonl(t_int); //Convert long from Host Byte Order to Network Byte Order
	if (!sendAll((char*)&t_int, sizeof(int))) //If int failed to send due to connection issue 
		return false; //Return false: Connetion issue, int not successfully sent
	return true; //Return true: int successfully sent
}

bool Client::recieveAll(char* data, int totalBytes)
{
	int bytesRecieved = 0; //Holds total bytes recieved
	while (bytesRecieved < totalBytes) // While we still have more bytes to recieve
	{
		int RetnCheck = recv(Connection, data + bytesRecieved, totalBytes - bytesRecieved, NULL); //try to recieve remaining bytes
		if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to recieve
			return false; //Return false, failed to recieveAll
		bytesRecieved += RetnCheck;
	}

	return true; //Success!
}

bool Client::GetInt(int& t_int)
{
	if (!recieveAll((char*)&t_int, sizeof(int))) //Try to receive int, If there is a connection issue 
		return false; //Return false: did not recieve int
	t_int = ntohl(t_int); //Convert long from Network Byte Order to Host Byte Order
	return true; //Return true: int successfully retrieved
}

bool Client::SendPacketType(PacketType t_packetType)
{
	if (!SendInt((int)t_packetType)) //If packettype failed to send due to connection issue 
		return false; //Return false: Connetion issue, packettype not successfully sent
	return true; //Return true: packettype successfully sent
}

bool Client::SendTagPlayer(int t_idToTag)
{
	if (!SendPacketType(PacketType::TagPlayer)) //Send Packet type: Chat Message
		return false; //Return false: Failed to send string
	PacketStructs::TagPlayer tagPlayer(t_idToTag);
	Packet p = tagPlayer.toPacket();
	sendAll(p.buffer, p.size);
	//if (!SendInt(t_idToTag)) //Send length of string, if sending buffer length failed,
	//	return false; //failed to send string buffer length, return false
	return true;
}

bool Client::GetPacketType(PacketType& t_packetType)
{
	int packetType;
	if (!GetInt(packetType)) //Try to receive packettype, If there is a connection issue 
		return false; //Return false: did not recieve packettype
	//packetType = ntohl(packetType);
	t_packetType = (PacketType)packetType;
	return true; //Return true: packettype successfully retrieved
}

bool Client::SendString(std::string& _string)
{
	if (!SendPacketType(PacketType::ChatMessage)) //Send Packet type: Chat Message
		return false; //Return false: Failed to send string
	int bufferLength = _string.size(); //find string buffer length
	if (!SendInt(bufferLength)) //Send length of string, if sending buffer length failed,
		return false; //failed to send string buffer length, return false
	if (!sendAll((char*)_string.c_str(), bufferLength)) //If packet failed to send due to connection issue 
		return false; //Return false: Connetion issue
	return true; //Return true: packet successfully sent
}

bool Client::GetString(std::string& _string)
{
	int bufferLength; // Holds length of message
	if (!GetInt(bufferLength)) //Get length of buffer and store it: bufferLength
		return false; //If get int fails, return false
	char* buffer = new char[bufferLength + 1]; //Allocate buffer
	buffer[bufferLength] = '\0'; // Set last char to null terminator so no junk
	
	if (!recieveAll(buffer, bufferLength)) // recieve message and store in buffer
	{
		delete[] buffer; //delete buffer to prevent memory leak
		return false; //return false: Fails to recieve string buffer
	}

	_string = buffer; //set string to recieved buffer mesage
	delete[] buffer; //deallocate buffer memory (cleanup to prevent memory leak)
	return true; //Return true if we were successful in retrieving the string
}

bool Client::GetPosition(sf::Vector2f& t_pos)
{
	float xPos;
	if (!recieveAll((char*)&xPos, sizeof(float))) //Try to receive float, If there is a connection issue 
		return false; //Return false: did not recieve float
	xPos = ntohl(xPos); //Convert long from Network Byte Order to Host Byte Order
	float yPos;
	if (!recieveAll((char*)&yPos, sizeof(float))) //Try to receive float, If there is a connection issue 
		return false; //Return false: did not recieve float
	yPos = ntohl(yPos); //Convert long from Network Byte Order to Host Byte Order
	t_pos.x = xPos;
	t_pos.y = yPos;
	return true; //Return true: int successfully retrieved
}
