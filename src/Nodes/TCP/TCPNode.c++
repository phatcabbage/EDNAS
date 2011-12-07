#include <cstring>
#include "TCPSocket.h++"
#include "TCPNode.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Nodes::TCPNode>::
    Logger("TCPNode");
}

namespace Nodes
{
    Logging::OStreamLogger
    TCPLog::Log("<TCP>");

    TCPNode::
    TCPNode(IPNode& n)
	: Node_(n)
	, Sockets_()
	, ReceivedAdapter_(*this, &TCPNode::Received)
	, NextDynamicPort_(TCP::MIN_UNPRIVILEGED_PORT)
    {
	TCPLog::Log << "Created TCP Node at " << n << Logging::endl;
	Node_[IPv4::TCP] += this->ReceivedAdapter_;
    }

    TCPNode::
    ~TCPNode()
    {
	Node_[IPv4::TCP] -= this->ReceivedAdapter_;
	TCPLog::Log << "Destroyed TCP Node at " << Node_ << Logging::endl;
    }
    
    TCP::Port
    TCPNode::
    Bind(TCPSocket& s, TCP::AddressType port)
    {
	TCPLog::Log << "Binding socket." << Logging::endl;
	if (!port) // port = 0, dynamically assign
	{
	    port = DynamicPort();
	    TCPLog::Log << "[Port " << port << "] dynamically chosen." << Logging::endl;
	    if (!port) return port;
	}
	    
	if (Sockets_.count(port)) return TCP::Port(0);
	
	Sockets_[port] = &s;
	TCPLog::Log << "[Port " << port << "] bound to [Socket " 
		    << s.Address() << "]" << Logging::endl;
	return port;
    }

    bool
    TCPNode::
    Bound(TCP::AddressType port) const
    {
	return (Sockets_.count(port));
    }

    bool
    TCPNode::
    Unbind(TCPSocket& s, TCP::AddressType port)
    {
	auto it = Sockets_.find(port);
	if (it->second != &s) return false;
	
	Sockets_.erase(it);
	return true;
    }

    void
    TCPNode::
    Send(TCP::PacketPtr& pkt, IPv4::AddressType ip)
    {
	auto ippkt = std::make_shared<IPv4::Packet>
	    (IPv4::TCP, pkt, pkt->Size(), ip, Node_.Address());
	TCP::Socket s = { this->Address(), pkt->Sender };
	TCP::Socket d = { ip, pkt->Receiver };
	TCPLog::Log << "Sending packet [" << s << "]->[" << d << "]" << Logging::endl;
	Node_.Send(ippkt);
    }

    TCP::AddressType
    TCPNode::
    DynamicPort()
    {
	if (NextDynamicPort_ > TCP::MAX_PORT) 
	    NextDynamicPort_ = TCP::MIN_UNPRIVILEGED_PORT;

	auto it = Sockets_.find(NextDynamicPort_), end = Sockets_.end();
	if (it == end) return NextDynamicPort_++;
	
	auto start = NextDynamicPort_;
	TCP::Port current;
	for (current = start.Value + 1; current != start; ++current)
	{
	    if (current > TCP::MAX_PORT)
		current = TCP::MIN_UNPRIVILEGED_PORT;

	    if ((++it)->first != current)
	    {
		NextDynamicPort_ = current;
		return NextDynamicPort_++;
	    }
	}
	return 0;
    }

    void
    TCPNode::
    Received(IPNode::MessageArg& msg)
    {
	Logger << "Received packet." << Logging::endl;
	auto tcppkt = std::static_pointer_cast<TCP::PacketType>
	    (msg.IPPkt->Payload);
	
	auto it = Sockets_.find(tcppkt->Receiver);
	if (it == Sockets_.end()) return;

	TCP::Socket sender = { msg.IPPkt->Sender, tcppkt->Sender };
	it->second->Received(sender, tcppkt);
    }
}
