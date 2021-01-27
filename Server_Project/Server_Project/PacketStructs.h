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
}