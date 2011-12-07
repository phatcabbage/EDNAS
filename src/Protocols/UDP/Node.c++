#include <cstdlib> // random()
#include "Node.h++"

namespace UDP
{
    IPv4::Protocol::AddressType
    Node::
    Address() const
    { 
	return (this->Sender_) ? 
	    this->Sender_->Address() :
	    IPv4::Protocol::INVALID_ADDRESS;
    }
    
    void
    Node::
    Sender(IPv4::Node* s)
    { 
	if (this->Sender_) (*this->Sender_)[Protocol::ID] -= *this;
	this->Sender_ = s;
	if (this->Sender_) (*this->Sender_)[Protocol::ID] += *this;
    }
    
    void
    Node::
    Received(Network::PacketListPtr& pktList)
    {
	auto& pkt = (*pktList)[Protocol::Layer];

	auto recvit = Receivers_.find(pkt->PayloadType);
	if (recvit != Receivers_.end())
	    recvit->second(pktList);
	else
	    Drop(pktList);
    }
}
