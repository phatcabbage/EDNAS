#ifndef NODES__PINGER_HXX__
#define NODES__PINGER_HXX__

#include "Core/Signaling/EventAdapter.h++"
#include "Logging/Logged.h++"
#include "./IPNode.h++"

namespace Nodes
{

    struct Pinger : Logging::Logged<Pinger>
    {
	IPNode&
	Node() const
	{ return this->Node_; }

	void SendPing(IPv4::AddressType ip);
	void ReceivedPing(IPv4::AddressType ip);
	void SendPong(IPv4::AddressType ip);
	void ReceivedPong(IPv4::AddressType ip);

	Pinger(IPNode& node);
	~Pinger();

    protected:
	IPNode& 
	Node_;
	
	EventAdapter<Pinger,IPNode::MessageArg&>
	ReceivedAdapter_;

	void Received(IPNode::MessageArg& msg);
    };
}

#endif // NODES__PINGER_HXX__
