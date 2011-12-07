#ifndef ETHERNET__PACKET_HXX__
#define ETHERNET__PACKET_HXX__

#include "SEDNAS.h++"
#include "Protocol.h++"
#include "Network/Packet.h++"

namespace Ethernet
{
    struct Packet : public Network::IPacket
    {
	static const SizeType 
	HEADER_SIZE = 304 * Bandwidth::BIT;
    
	Packet(int payloadType,
	       int receiver = Protocol::INVALID_ADDRESS,
	       int sender = Protocol::INVALID_ADDRESS,
	       SizeType size = 0)
	    : Network::IPacket(Protocol::ID, payloadType, receiver, 
			       sender, size + HEADER_SIZE, false)
	{}

	static 
	Network::PacketListPtr&
	Wrap(Network::PacketListPtr& list,
	     int receiver = Protocol::INVALID_ADDRESS,
	     int sender = Protocol::INVALID_ADDRESS)
	{
	    auto& nextpkt = (*list)[Protocol::Layer + 1];
	    int type = nextpkt ? nextpkt->Type : Protocols::FAKE;
	    int length = nextpkt ? nextpkt->Length : 0;
	    (*list)[Protocol::Layer] = 
		std::make_shared<Packet>
		(type, receiver, sender, length);

	    return list;
	}

	virtual ~Packet(){}
    };
}

#endif // ETHERNET__PACKET_HXX__
