#ifndef UDP__PACKET_HXX__
#define UDP__PACKET_HXX__

#include "Protocol.h++"
#include "Network/Packet.h++"

namespace UDP
{
    struct Packet : public Network::IPacket
    {
	static const SizeType
	HEADER_SIZE = 32 * Bandwidth::BIT;

	static Network::PacketListPtr&
	Wrap(Network::PacketListPtr& pktList, Protocol::AddressType receiver,
	     Protocol::AddressType sender)
	{
	    auto& wrappedpkt = (*pktList)[Protocol::Layer + 1];
	    int type = wrappedpkt ? wrappedpkt->Type : Protocols::FAKE;
	    int length = wrappedpkt ? wrappedpkt->Length : 0;

	    auto& pkt = (*pktList)[Protocol::Layer];
	    pkt = std::make_shared<Packet>
		(type, receiver, sender, length);

	    return pktList;
	}

	Packet(int type,
	       Protocol::AddressType receiver = Protocol::AddressType(),
	       Protocol::AddressType sender = Protocol::AddressType(),
	       SizeType size = 0)
	    : Network::IPacket(Protocol::ID, type, receiver, sender, 
			       HEADER_SIZE + size, false)
	{}
	virtual ~Packet(){}
    };
}

#endif // UDP__PACKET_HXX__
