#ifndef NODES__TCPNODE_HXX__
#define NODES__TCPNODE_HXX__

#include <map>
#include <utility>

#include "Core/Signaling/EventAdapter.h++"
#include "Logging/Logged.h++"
#include "Protocols/TCP.h++"
#include "Protocols/IPv4.h++"

#include "Nodes/IPNode.h++"

namespace Nodes
{
    using Protocols::IPv4;
    using Protocols::TCP;
    
    struct TCPSocket;

    struct TCPLog
    {
	static 
	Logging::OStreamLogger
	Log;
    };

    struct TCPNode : public Logging::Logged<TCPNode>
    {
	IPv4::AddressType
	Address() const
	{ return this->Node_.Address(); }

	IPNode&
	Interface() const
	{ return this->Node_; }

	TCP::Port
	Bind(TCPSocket& s, TCP::Port port = 0);

	bool
	Bound(TCP::AddressType port) const;

	bool
	Unbind(TCPSocket& s, TCP::AddressType port);

	TCPNode(IPNode& n);
	~TCPNode();

	void
	Send(TCP::PacketPtr& pkt, IPv4::AddressType ip);

    protected:
	IPNode& 
	Node_;

	std::map<TCP::AddressType,TCPSocket*>
	Sockets_;

	TCP::AddressType
	NextDynamicPort_;

	TCP::AddressType
	DynamicPort();

	void 
	Received(IPNode::MessageArg& pkt);
    };
}

#endif // NODES__TCPNODE_HXX__
