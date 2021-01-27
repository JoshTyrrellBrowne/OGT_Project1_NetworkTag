#include "PacketStructs.h"

PacketStructs::ChatMessage::ChatMessage(std::string t_msg)
{
	message = t_msg;
}

Packet PacketStructs::ChatMessage::toPacket()
{
	const int packetSize = sizeof(int) * 2 + message.size() * sizeof(char); //calculate packet size
	char* buffer = new char[packetSize];
	int packetType = htonl((int)PacketType::ChatMessage);
	int messageSize = htonl(message.size());
	memcpy(buffer, &packetType, sizeof(int)); //copy packet type
	memcpy(buffer + sizeof(int), &messageSize, sizeof(int)); //copy packet size
	memcpy(buffer + sizeof(int) * 2, message.c_str(), message.size() * sizeof(char));
	Packet packet(buffer, packetSize);
	return packet;
}
