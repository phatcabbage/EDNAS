#include "NodeBase.h++"
#include "Protocols/Ethernet.h++"
#include "Protocols/IPv4.h++"

namespace ARP
{
    MACAddress
    NodeBase::
    EthernetAddress() const
    { 
	return this->EtherNode_ ? 
	    this->EtherNode_->Address() :
	    Ethernet::Protocol::INVALID_ADDRESS; 
    }
    
    IPAddress
    NodeBase::
    IPv4Address() const
    { 
	return this->IPNode_ ? 
	    this->IPNode_->Address() : 
	    IPv4::Protocol::INVALID_ADDRESS; 
    }

    void
    NodeBase::
    EthernetNode(Ethernet::Node* hw)
    {
	auto type = Protocol::ID;
	if (this->EtherNode_) (*this->EtherNode_)[type] -= *this;
	this->EtherNode_ = hw;
	if (this->EtherNode_) (*this->EtherNode_)[type] += *this;
    }

    void
    NodeBase::
    Received(Network::PacketListPtr& pktList)
    {
	Network::PacketPtr& pktptr = (*pktList)[Protocol::Layer];
	ARP::Packet* pkt = static_cast<ARP::Packet*>(pktptr.get());
	MACAddress senderMAC = pkt->SenderMAC;
	MACAddress receiverMAC = pkt->ReceiverMAC;

	if (pkt->Sender == this->IPv4Address())
	    return;

	this->UpdateTable(pkt->Sender, senderMAC, ResolutionArg::VALID);

	if (pkt->Receiver != this->Address())
	{
	    if (pkt->Receiver != IPv4::Protocol::INVALID_ADDRESS &&
		pkt->Receiver != IPv4::Protocol::BROADCAST_ADDRESS &&
		pkt->Receiver != IPv4::Protocol::DEFAULT_ADDRESS &&
		receiverMAC != Ethernet::Protocol::INVALID_ADDRESS &&
		receiverMAC != Ethernet::Protocol::BROADCAST_ADDRESS &&
		receiverMAC != Ethernet::Protocol::DEFAULT_ADDRESS)
		this->UpdateTable(pkt->Receiver, receiverMAC,
				  ResolutionArg::VALID);
	}
	else // it was for us
	{
	    if (pkt->PayloadType == Protocol::Request)
	    {
		Network::PacketListPtr reply = 
		    Packet::CreateReply(*pkt, this->EthernetAddress());
		if (this->EtherNode_)
		    this->EtherNode_->Send(reply, senderMAC);
	    }
	}
    }
}
