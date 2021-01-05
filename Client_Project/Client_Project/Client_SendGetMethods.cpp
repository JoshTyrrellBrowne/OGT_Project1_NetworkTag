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
	int RetnCheck = send(Connection, (char*)&t_int, sizeof(int), NULL); //send int
	if (RetnCheck == SOCKET_ERROR) //If int failed to send due to connection issue 
		return false; //Return false: Connetion issue
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
	int RetnCheck = recv(Connection, (char*)&t_int, sizeof(int), NULL); //recieve int
	if (RetnCheck == SOCKET_ERROR) //If there is a connection issue 
		return false; //Return false: did not recieve int
	return true; //Return true: int successfully retrieved
}

bool Client::SendPacketType(Packet t_packetType)
{
	int RetnCheck = send(Connection, (char*)&t_packetType, sizeof(int), NULL); //send packet
	if (RetnCheck == SOCKET_ERROR) //If packet failed to send due to connection issue 
		return false; //Return false: Connetion issue
	return true; //Return true: packet successfully sent
}

bool Client::GetPacketType(Packet& t_packetType)
{
	int RetnCheck = recv(Connection, (char*)&t_packetType, sizeof(int), NULL); //recieve packet
	if (RetnCheck == SOCKET_ERROR) //If there is a connection issue 
		return false; //Return false: did not recieve packet
	return true; //Return true: packet successfully retrieved
}

bool Client::SendString(std::string& _string)
{
	if (!SendPacketType(P_ChatMessage)) //Send Packet type: Chat Message
		return false; //Return false: Failed to send string
	int bufferLength = _string.size(); //find string buffer length
	if (!SendInt(bufferLength)) //Send length of string, if sending buffer length failed,
		return false; //failed to send string buffer length, return false
	int RetnCheck = send(Connection, _string.c_str(), bufferLength, NULL); //send string
	if (RetnCheck == SOCKET_ERROR) //If packet failed to send due to connection issue 
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
	int RetnCheck = recv(Connection, buffer, bufferLength, NULL); //receive message and store
	_string = buffer; //set string to recieved buffer mesage
	delete[] buffer; //deallocate buffer memory (cleanup to prevent memory leak)
	if (RetnCheck == SOCKET_ERROR) //If connection is lost while getting message
		return false; //If there is an issue with connection, return false
	return true; //Return true if we were successful in retrieving the string
}