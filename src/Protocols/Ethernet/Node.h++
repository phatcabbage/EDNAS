#ifndef PROTOCOLS__ETHERNET__NODE_HXX__
#define PROTOCOLS__ETHERNET__NODE_HXX__

#include <iostream>
#include "NodeBase.h++"
#include "Protocols/Ethernet/Packet.h++"

#include "Network/Link.h++"

namespace Ethernet
{
    struct Node : public NodeBase
    {
	Event<Network::PacketListPtr&>&
	operator[](int id)
	{ return this->Receivers_[id]; }

	Node(SenderType s = SenderType());
	
	virtual ~Node(){}
    protected:
	std::map<int,MultiEvent<Network::PacketListPtr&> >
	Receivers_;

	virtual void
	Received(Network::PacketListPtr& pktList);

	void
	Drop(Network::PacketListPtr& pktList){}
    };
}

#endif // PROTOCOLS__ETHERNET__NODE_HXX__
