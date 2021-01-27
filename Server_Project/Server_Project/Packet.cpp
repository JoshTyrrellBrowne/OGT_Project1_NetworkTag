#include "Packet.h"

Packet::Packet()
{
}

Packet::Packet(char* t_buffer, int t_size)
{
	buffer = t_buffer;
	size = t_size;
}

Packet::Packet(const Packet& p) //Allocate new block for buffer
{
	size = p.size;
	buffer = new char[size];
	memcpy(buffer, p.buffer, size);
}
