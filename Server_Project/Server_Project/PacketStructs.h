#pragma once
#include "PacketType.h"
#include "Packet.h"
#include <string>
#include <Windows.h>

namespace PacketStructs
{
	class ChatMessage 
	{
	public:
		ChatMessage(std::string);
		Packet toPacket(); //converts Chat Message to Packet
	private:
		std::string message;
	};

	class SetID
	{
	public:
		SetID(int t_ID);
		Packet toPacket(); //converts Chat Message to Packet
	private:
		int m_ID;
	};

	class SetPosition
	{
	public:
		SetPosition(int t_id, float t_xPos, float t_yPos);
		Packet toPacket(); //converts Chat Message to Packet
	private:
		int m_id; //the id of player 
		float m_xPos; //the x coordinate
		float m_yPos; //the y coordinate
	};
}