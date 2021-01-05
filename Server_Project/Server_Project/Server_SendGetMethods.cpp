#include "Server.h"


bool Server::sendAll(int ID, char* data, int totalBytes)
{
	int bytesSent = 0; //Holds total bytes sent
	while (bytesSent < totalBytes) // While we still have more bytes to send
	{
		int RetnCheck = send(Connections[ID], data + bytesSent, totalBytes - bytesSent, NULL); //try to send remaining bytes
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
		int RetnCheck = recv(Connections[ID], data + bytesRecieved, totalBytes - bytesRecieved, NULL); //try to recieve remaining bytes
		if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to recieve
			return false; //Return false, failed to recieveAll
		bytesRecieved += RetnCheck; // Add to total bytes recieved
	}
	return true; //Success!
}

bool Server::SendInt(int ID, int t_int)
{
	if (!sendAll(ID, (char*)&t_int, sizeof(int))) //If int failed to send due to connection issue 
		return false; //Return false: Connetion issue, int not successfully sent
	return true; //Return true: int successfully sent
}

bool Server::GetInt(int ID, int& t_int)
{
	if (!recieveAll(ID, (char*)&t_int, sizeof(int))) //Try to receive int, If there is a connection issue 
		return false; //Return false: did not recieve int
	return true; //Return true: int successfully retrieved
}

bool Server::SendPacketType(int ID, Packet t_packetType)
{
	if (!sendAll(ID, (char*)&t_packetType, sizeof(Packet))) //If packettype failed to send due to connection issue 
		return false; //Return false: Connetion issue, packettype not successfully sent
	return true; //Return true: packettype successfully sent
}

bool Server::GetPacketType(int ID, Packet& t_packetType)
{
	if (!recieveAll(ID, (char*)&t_packetType, sizeof(Packet))) //Try to receive packettype, If there is a connection issue 
		return false; //Return false: did not recieve packettype
	return true; //Return true: packettype successfully retrieved
}

bool Server::SendString(int ID, std::string& _string)
{
	if (!SendPacketType(ID, P_ChatMessage)) //Send Packet type: Chat Message
		return false; //Return false: Failed to send string
	int bufferLength = _string.size(); //find string buffer length
	if (!SendInt(ID, bufferLength)) //Send length of string, if sending buffer length failed,
		return false; //failed to send string buffer length, return false
	if (!sendAll(ID, (char*)_string.c_str(), bufferLength)) //If packet failed to send due to connection issue 
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

	if (!recieveAll(ID, buffer, bufferLength)) // recieve message and store in buffer
	{
		delete[] buffer; //delete buffer to prevent memory leak
		return false; //return false: Fails to recieve string buffer
	}

	_string = buffer; //set string to recieved buffer mesage
	delete[] buffer; //deallocate buffer memory (cleanup to prevent memory leak)
	return true; //Return true if we were successful in retrieving the string
}