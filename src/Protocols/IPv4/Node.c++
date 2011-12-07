#include <iostream>
#include "Protocols/Ethernet.h++"
#include "Protocols/ARP.h++"
#include "Packet.h++"
#include "Node.h++"

#include "Logging/OStreamLogger.h++"

namespace IPv4
{
    void
    Node::
    Received(Network::PacketListPtr& pktList)
    {
	auto& pkt = (*pktList)[Protocol::Layer];
	// Protocol::Log << "[IPNode " << this->Address() << "] Received packet:\n"
	// 	  << "\tType:     "
	// 	  << Protocols::ProtocolName[pkt->PayloadType] << "\n"
	// 	  << "\tSender:   " << pkt->Sender << "\n"
	// 	  << "\tReceiver: " << pkt->Receiver << Logging::endl;

	Protocol::AddressType receiver = pkt->Receiver;
	
	if (!IsLocalHost(receiver) && !IsBroadcast(receiver))
	{
	    // Protocol::Log << "[IPNode] Not addressed to this node. Dropping."
	    // 	      << Logging::endl;
	    Drop(pktList);
	    return;
	}
	    
	auto it = this->Receivers_.find(pkt->PayloadType);
	if (it != this->Receivers_.end())
	    it->second(pktList);
	else
	    Drop(pktList);
    }

    void
    Node::
    Drop(Network::PacketListPtr& pktList) const
    {
	if (!pktList->empty())
	{
	    auto& pkt = (*pktList)[Protocol::Layer];
	    auto receiver = pkt->Receiver;
	    auto sender = pkt->Sender;
	    int type = pkt->PayloadType;
	    Protocol::Log << "Dropping packet.\n";
	    Protocol::Log << "\tFrom: " << sender << "\n";
	    Protocol::Log << "\tTo:   " << receiver << "\n";
	    Protocol::Log << "\tType: " << type << Logging::endl;
	}
    }

    Node::
    Node(Ethernet::Node* s,
	 Protocol::AddressType	ip,
	 Protocol::Netmask	nm,
	 Protocol::AddressType	gw)
	: NodeBase(s, ip, nm, gw)
	, Receivers_()
    {}
}
