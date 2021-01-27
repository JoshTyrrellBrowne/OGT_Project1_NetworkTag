#include "PacketManager.h"

bool PacketManager::HasPendingPackets()
{
    return (queue_packets.size() > 0); //returns true if size > 0
}

void PacketManager::Append(Packet p)
{
    std::lock_guard<std::mutex> lock(mutex_packets); //lock mutex so two threads dont access data at the same time
    queue_packets.push(p);
}

Packet PacketManager::Retrieve()
{
    std::lock_guard<std::mutex> lock(mutex_packets); //lock mutex so two threads dont access data at the same time
    Packet p = queue_packets.front(); //get packet from front of queue
    queue_packets.pop(); //removes packet from front queueu

    return p; //return packet thats removed from queue
}
