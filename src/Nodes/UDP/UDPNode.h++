#ifndef NODES__UDPNODE_HXX__
#define NODES__UDPNODE_HXX__

#include <iosfwd>
#include <map>

#include "Core/Signaling/EventAdapter.h++"
#include "Logging/Logged.h++"

#include "Protocols/IPv4.h++"
#include "Protocols/UDP.h++"

#include "IPNode.h++"

namespace Nodes
{
    using Protocols::IPv4;
    using Protocols::UDP;
    
    struct UDPNode : public Logging::Logged<UDPNode>
    {
	struct MessageArg;
	typedef SingleEvent<MessageArg&> MessageEvent;

	bool
	Bind(EventHandler<MessageArg&>& handler,
	     UDP::Port port);
	
	bool
	Unbind(UDP::Port port);

	bool
	Bound(UDP::Port port) const;

	IPv4::AddressType
	Address() const
	{ return this->Node_.Address(); }

	IPNode&
	Node()
	{ return this->Node_; }

	void
	Send(UDP::PacketPtr& pkt,
	     IPv4::AddressType rcv)
	{ this->Send(pkt, rcv, this->Address()); }

	void
	Send(UDP::PacketPtr& pkt, 
	     IPv4::AddressType rcv, 
	     IPv4::AddressType snd);

	UDPNode(IPNode& node)
	    : Node_(node)
	    , Handlers_()
	    , ReceivedAdapter_(*this, &UDPNode::Received)
	    , IncomingPackets()
	{
	    this->Associate();
	}

    protected:
	IPNode& Node_;
	
	std::map<UDP::Port, MessageEvent>
	Handlers_;
	    
	EventAdapter<UDPNode, IPNode::MessageArg&>
	ReceivedAdapter_;

	std::map<IPv4::PacketPtr, std::set<int> >
	IncomingPackets;

	void
	Associate()
	{
	    this->Node_[IPv4::UDP] += this->ReceivedAdapter_;
	}

	void
	Dissociate()
	{
	    this->Node_[IPv4::UDP] -= this->ReceivedAdapter_;
	}

	void
	Received(IPNode::MessageArg& msg);

	void
	Received(UDP::PacketPtr& pkt);
    };

    struct UDPNode::MessageArg
    {
	IPv4::PacketPtr	IPPkt;
	UDP::PacketPtr	UDPPkt;
	MessageArg(IPv4::PacketPtr& ippkt,
		   UDP::PacketPtr&  udppkt)
	    : IPPkt(ippkt)
	    , UDPPkt(udppkt)
	{}
    };
}

std::ostream&
operator<<(std::ostream& o, const Nodes::UDPNode& n);

#endif // NODES__UDPNODE_HXX__
