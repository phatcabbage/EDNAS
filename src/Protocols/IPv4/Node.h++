#ifndef IPV4__NODE_HXX__
#define IPV4__NODE_HXX__

#include "SEDNAS.h++"

#include "Protocol.h++"
#include "NodeBase.h++"
#include "Network/Packet.h++"

#include "Protocols/ProtocolList.h++"

namespace IPv4
{
    struct Node : public NodeBase
    {
	Event<Network::PacketListPtr&>&
	operator[](Protocols::ProtocolID id)
	{ return Receivers_[id]; }
	
	virtual void
	Received(Network::PacketListPtr& pktList);

	Node(Ethernet::Node*		s = NULL,
	     Protocol::AddressType	ip = Protocol::DEFAULT_ADDRESS,
	     Protocol::Netmask		nm = Protocol::CLASS_C,
	     Protocol::AddressType	gw = Protocol::INVALID_ADDRESS);

	virtual ~Node(){}

    protected:
	std::map<int,SingleEvent<Network::PacketListPtr&> >
	Receivers_;

	void
	Drop(Network::PacketListPtr& pkt) const;
    };
}

#endif // IPV4__NODE_HXX__
