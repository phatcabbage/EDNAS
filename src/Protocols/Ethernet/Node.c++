#include <iostream>
#include "Node.h++"

namespace Ethernet
{
    Node::Node(SenderType s)
	: NodeBase(s)
	, Receivers_()
    {	}
    
    void
    Node::
    Received(Network::PacketListPtr& pktList)
    {
	auto& pkt = (*pktList)[Protocol::Layer];
	// Protocol::Log << "[EthNode] Received packet:\n"
	// 	      << "\tType:     " 
	// 	      << Protocols::ProtocolName[pkt->PayloadType] << "\n"
	// 	      << "\tSender:   " << pkt->Sender << "\n"
	// 	      << "\tReceiver: " << pkt->Receiver << Logging::endl;
	// drop empty packets
	if (pktList->empty()) 
	{
	    Drop(pktList);
	    return;
	}

	// drop packets that aren't Ethernet
	if (pkt->Type != (int)Protocol::ID) 
	{
	    // Protocol::Log << "[EthNode] Dropping because packet type is not "
	    // 		  << "Ethernet." << Logging::endl;
	    Drop(pktList);
	    return;
	}

	// pass on message
	auto it = this->Receivers_.find(pkt->PayloadType);
	if (it != this->Receivers_.end())
	{
	    // Protocol::Log << "[EthNode] Found! Routing." << Logging::endl;
	    (it->second)(pktList);
	}
	else
	{
	    Drop(pktList);
	    // Protocol::Log << "[EthNode] Handler not found! Dropping." << Logging::endl;
	}
    }
}
