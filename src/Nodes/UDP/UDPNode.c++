#include <cstdlib> // random()
#include "UDPNode.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Nodes::UDPNode>::
    Logger("UDPNode");
}

namespace Nodes
{
    bool
    UDPNode::
    Bind(EventHandler<MessageArg&>& handler,
	 UDP::Port port)
    {
	if (this->Handlers_.count(port))
	    return false;
	
	this->Handlers_[port] += handler;
	return true;
    }

    bool
    UDPNode::
    Unbind(UDP::Port port)
    {
	return (Handlers_.erase(port) > 0);
    }

    bool
    UDPNode::
    Bound(UDP::Port port) const
    {
	return (Handlers_.count(port) > 0);
    }

    void 
    UDPNode::
    Send(UDP::PacketPtr& pkt, IPv4::AddressType rcv, IPv4::AddressType snd)
    {
	Logger << *this << "[Packet " << snd << ":" 
	       << pkt->Sender << "] --> [Packet " << rcv << ":"
	       << pkt->Receiver << "]"
	       << Logging::endl;

// 	if (pkt->Size() < IPv4::PacketType::MTU())
// 	{
	auto ippkt = std::make_shared<IPv4::PacketType>
		(IPv4::UDP, pkt, pkt->Size());
	    ippkt->Receiver = rcv;
	    ippkt->Sender = snd;
	    this->Node_.Send(ippkt);
// 	}
// 	else
// 	{
// 	    auto frags = IPv4::PacketType::Package(IPv4::UDP, pkt,
// 						   pkt->Size());
// 	    for(auto it = frags.begin(), end = frags.end();
// 		it != end; ++it)
// 	    {
// 		(*it)->Receiver = rcv;
// 		(*it)->Sender= snd;
// 		this->Node_.Send(*it);
// 	    }
// 	}
    }

    void
    UDPNode::
    Received(IPNode::MessageArg& msg)
    {
	IPv4::PacketPtr& ippkt = msg.IPPkt;

	auto udppkt = std::static_pointer_cast<UDP::PacketType>
	    (ippkt->Payload);

	Logger << *this << "Received packet from " << msg.IPPkt->Sender
	       << ":" << udppkt->Sender << Logging::endl;
	Logger << *this << "\tAddressed to " << msg.IPPkt->Receiver
	       << ":" << udppkt->Receiver << Logging::endl;
	Logger << *this << "\tOf type " << udppkt->Type << Logging::endl;

	MessageArg udpmsg(ippkt, udppkt);
	this->Handlers_[udppkt->Receiver](udpmsg);
    }
}

std::ostream&
operator<<(std::ostream& o, const Nodes::UDPNode& n)
{ 
    Protocols::IPv4::AddressType ip = n.Address();
    o << "[";
    o << ip;
    o << "]";
    return o;
}
