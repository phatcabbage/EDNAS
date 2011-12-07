#ifndef IPV4__PACKET_HXX__
#define IPV4__PACKET_HXX__

#include "Protocol.h++"
#include "Network/Packet.h++"

namespace IPv4
{
    struct Packet : public Network::IPacket
    {
	static const SizeType 
	HEADER_SIZE = 160 * Bandwidth::BIT;

	static 
	Network::PacketListPtr&
	Wrap(Network::PacketListPtr& list,
	     Protocol::AddressType receiver = Protocol::INVALID_ADDRESS,
	     Protocol::AddressType sender   = Protocol::INVALID_ADDRESS)
	{
	    auto& wrappedpkt = (*list)[Protocol::Layer + 1];
	    int type = wrappedpkt ? wrappedpkt->Type : Protocols::FAKE;
	    SizeType length = wrappedpkt ? wrappedpkt->Length : 0;
	    (*list)[Protocol::Layer] = std::make_shared<Packet>
		(type, receiver, sender, length);
	    return list;
	}

	Packet(int payloadType,
	       Protocol::AddressType receiver = Protocol::DEFAULT_ADDRESS,
	       Protocol::AddressType sender = Protocol::DEFAULT_ADDRESS,
	       SizeType size = 0)
	    : Network::IPacket(Protocol::ID, payloadType, receiver, 
			       sender, size + HEADER_SIZE, false)
	{
	}

	virtual ~Packet(){}
    };
}

#endif // IPV4__PACKET_HXX__
