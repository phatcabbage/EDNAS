#include "Protocols/ICMP.h++"
#include "./Pinger.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Nodes::Pinger>::
    Logger("Pinger");
}

namespace Nodes
{
    using Protocols::ICMP;

    Pinger::
    Pinger(IPNode& node)
	: Node_(node)
	, ReceivedAdapter_(*this, &Pinger::Received)
    { this->Node_[IPv4::ICMP] += this->ReceivedAdapter_; }

    Pinger::
    ~Pinger()
    { this->Node_[IPv4::ICMP] -= this->ReceivedAdapter_; }

    void
    Pinger::
    Received(IPNode::MessageArg& msg)
    {
	IPv4::PacketPtr& pkt = msg.IPPkt;
	Logger << "Received packet." << Logging::endl;
	if (pkt->Type == IPv4::ICMP)
	{
	    auto icmppkt = std::static_pointer_cast<ICMP::PacketType>
		(pkt->Payload);
	    switch(icmppkt->Type)
	    {
	    case ICMP::EchoRequest:
		ReceivedPing(pkt->Sender);
		break;
	    case ICMP::EchoReply:
		ReceivedPong(pkt->Sender);
		break;
	    default:
		break;
	    }
	}
    }
		
    void
    Pinger::
    SendPing(IPv4::AddressType ip)
    {
	auto icmppkt = std::make_shared<ICMP::PacketType>();
	*icmppkt = { ICMP::EchoRequest };
	auto pkt = std::make_shared<IPv4::PacketType>(IPv4::ICMP, icmppkt, icmppkt->Size());

	pkt->Sender = Node_.Address();
	pkt->Receiver = ip;
	Logger << "[" << ip << "] Sending PING (packet type " << pkt->Type
	       << ")" << Logging::endl;
	Node_.Send(pkt);
    }

    void
    Pinger::
    ReceivedPing(IPv4::AddressType ip)
    {
	Logger << "PING!" << Logging::endl;
	SendPong(ip);
    }

    void
    Pinger::
    SendPong(IPv4::AddressType ip)
    {
	auto icmppkt = std::make_shared<ICMP::PacketType>();
	*icmppkt = { ICMP::EchoReply };
	auto pkt = std::make_shared<IPv4::PacketType>(IPv4::ICMP, icmppkt, icmppkt->Size());
	// auto pkt = ippkts.front();
	pkt->Sender = Node_.Address();
	pkt->Receiver = ip;
	Node_.Send(pkt);
    }
    
    void
    Pinger::
    ReceivedPong(IPv4::AddressType ip)
    {
	Logger << "PONG!" << Logging::endl;
    }
}
