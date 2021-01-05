#include "Server.h"

bool Server::SendInt(int ID, int t_int)
{
	int RetnCheck = send(Connections[ID], (char*)&t_int, sizeof(int), NULL); //send int
	if (RetnCheck == SOCKET_ERROR) //If int failed to send due to connection issue 
		return false; //Return false: Connetion issue
	return true; //Return true: int successfully sent
}

bool Server::GetInt(int ID, int& t_int)
{
	int RetnCheck = recv(Connections[ID], (char*)&t_int, sizeof(int), NULL); //recieve int
	if (RetnCheck == SOCKET_ERROR) //If there is a connection issue 
		return false; //Return false: did not recieve int
	return true; //Return true: int successfully retrieved
}

bool Server::SendPacketType(int ID, Packet t_packetType)
{
	int RetnCheck = send(Connections[ID], (char*)&t_packetType, sizeof(Packet), NULL); //send packet
	if (RetnCheck == SOCKET_ERROR) //If packet failed to send due to connection issue 
		return false; //Return false: Connetion issue
	return true; //Return true: packet successfully sent
}

bool Server::GetPacketType(int ID, Packet& t_packetType)
{
	int RetnCheck = recv(Connections[ID], (char*)&t_packetType, sizeof(Packet), NULL); //recieve packet
	if (RetnCheck == SOCKET_ERROR) //If there is a connection issue 
		return false; //Return false: did not recieve packet
	return true; //Return true: packet successfully retrieved
}

bool Server::SendString(int ID, std::string& _string)
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

bool Server::GetString(int ID, std::string& _string)
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