#include <iostream>
#include "Logging/OStreamLogger.h++"
#include "Node.h++"

namespace TCP
{
    static 
    int 
    G_UID = 0;

    static 
    Logging::OStreamLogger
    Log("<TCP Node>");

    Node::
    Node(SenderType* s)
	: UID_(++G_UID)
	, Sender_(NULL)
	, Receivers_()
    {
	Log << "Beginning creation of " << *this << Logging::endl;
	this->Sender(s);
	Log << "Completed creation of " << *this << Logging::endl;
    }

    IPv4::Protocol::AddressType
    Node::
    IPAddress() const
    {
	if (Sender_)
	    return Sender_->Address();
	else
	    return IPv4::Protocol::INVALID_ADDRESS;
    }

    void
    Node::
    Sender(SenderType* s)
    {
	Log << "Changing sender for " << *this << Logging::endl;
	if (this->Sender_) (*this->Sender_)[Protocol::ID] -= *this;
	this->Sender_ = s;
	if (this->Sender_) (*this->Sender_)[Protocol::ID] += *this;
    }

    void
    Node::
    Received(PLP& pktList)
    {
	auto& pkt = (*pktList)[Protocol::Layer];
	Log << *this << " received packet.\n"
	    << "\tType:     " << pkt->Type
	    << "\tPayload:  " << pkt->PayloadType
	    << "\tSender:   " << pkt->Sender
	    << "\tReceiver: " << pkt->Receiver
	    << Logging::endl;

	if (pkt->Type != Protocol::ID)
	{
	    Log << "Packet is not the right type; dropping." << Logging::endl;
	    Drop(pktList);
	}
	else
	{
	    auto recvit = this->Receivers_.find(pkt->Receiver);
	    if (recvit != this->Receivers_.end())
		recvit->second(pktList);
	    else
	    {
		Log << "No receiver on port; dropping." << Logging::endl;
		Log << "Packet port is " << pkt->Receiver << "; receivers are on:";
		for (auto it = Receivers_.begin(), end = Receivers_.end(); it != end; ++it)
		    Log << "\n\t" << Port(it->first);
		Log << Logging::endl;
		Drop(pktList);
	    }
	}
    }

    void
    Node::
    Send(PLP& pktList, IPv4::Protocol::AddressType ip)
    {
	auto& pkt = (*pktList)[Protocol::Layer];
	Log << *this << " sending packet to " << ip << "\n"
	    << "\tSender port:   " << Port(pkt->Sender) << "\n"
	    << "\tReceiver port: " << Port(pkt->Receiver) << Logging::endl;

	if (this->Sender_)
	    this->Sender_->Send(pktList, ip);
    }

    void
    Node::
    Drop(Network::PacketListPtr& pktList)
    {
	Log << *this << " dropped packet." << Logging::endl;
    }
}

std::ostream&
operator<<(std::ostream& o, const TCP::Node& n)
{ return o << "[TCP #" << n.UID() << " @ " << n.IPAddress() << "]"; }
