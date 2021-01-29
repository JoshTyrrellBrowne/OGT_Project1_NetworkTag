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

PacketStructs::SetID::SetID(int t_ID)
{
	m_ID = t_ID;
}

Packet PacketStructs::SetID::toPacket()
{
	const int packetSize = sizeof(int) * 2; //calculate packet size (type,value)
	char* buffer = new char[packetSize];
	int packetType = htonl((int)PacketType::SetID);
	int message = htonl(m_ID);
	memcpy(buffer, &packetType, sizeof(int)); //copy packet type
	memcpy(buffer + sizeof(int), &message, sizeof(int)); //copy packet size
	//memcpy(buffer + sizeof(int) * 2, message.c_str(), message.size() * sizeof(char));
	Packet packet(buffer, packetSize);
	return packet;
}

PacketStructs::SetPosition::SetPosition(int t_id, float t_xPos, float t_yPos)
{
	m_id = t_id;
	m_xPos = t_xPos;
	m_yPos = t_yPos;
}

Packet PacketStructs::SetPosition::toPacket()
{
	const int packetSize = (sizeof(int) * 2) + (sizeof(float) * 2); //calculate packet size (type + id & 2 coords)
	char* buffer = new char[packetSize];
	int packetType = htonl((int)PacketType::SetPosition);
	int id = htonl(m_id);
	float xPos = htonl(m_xPos);
	float yPos = htonl(m_yPos);
	memcpy(buffer, &packetType, sizeof(int)); //copy packet type
	memcpy(buffer + sizeof(int), &id, sizeof(int)); //copy player id
	memcpy(buffer + (sizeof(int) * 2), &xPos, sizeof(float)); //copy xcoord
	memcpy(buffer + (sizeof(int) * 2) + sizeof(float), &yPos, sizeof(float));
	Packet packet(buffer, packetSize);
	return packet;
}

PacketStructs::TagPlayer::TagPlayer(int t_id)
{
	m_id = t_id;
}

Packet PacketStructs::TagPlayer::toPacket()
{
	const int packetSize = sizeof(int) * 2; //calculate packet size (type,value)
	char* buffer = new char[packetSize];
	int packetType = htonl((int)PacketType::TagPlayer);
	int id = htonl(m_id);
	memcpy(buffer, &packetType, sizeof(int)); //copy packet type
	memcpy(buffer + sizeof(int), &id, sizeof(int)); //copy packet size
	//memcpy(buffer + sizeof(int) * 2, message.c_str(), message.size() * sizeof(char));
	Packet packet(buffer, packetSize);
	return packet;
}
