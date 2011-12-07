#ifndef UDP__NODE_HXX__
#define UDP__NODE_HXX__

#include "SEDNAS.h++"
#include "Simulation/Event.h++"

#include "Protocols/IPv4.h++"
#include "Protocol.h++"
#include "Packet.h++"

namespace UDP
{
    struct Node : public EventHandler<Network::PacketListPtr&>
    {
	Event<Network::PacketListPtr&>&
	operator[](int id)
	{ return Receivers_[id]; }

	IPv4::Protocol::AddressType
	Address() const;

	IPv4::Node*
	Sender() const
	{ return this->Sender_; }

	void
	Sender(IPv4::Node* s);

	void
	Send(Network::PacketListPtr& pktList, IPv4::Protocol::AddressType receiver)
	{ 
	    Packet::Wrap(pktList, receiver, this->Address());
	    if (this->Sender_) this->Sender_->Send(pktList, receiver); 
	}

	void
	Send(Network::PacketListPtr& pktList, IPv4::Protocol::AddressType receiver,
	     IPv4::Protocol::AddressType sender)
	{
	    Packet::Wrap(pktList, receiver, sender);
	    if (this->Sender_) 
		this->Sender_->Send(pktList, receiver, sender); 
	}

	Node(IPv4::Node* s = NULL)
	    : Sender_(NULL)
	    , Receivers_()
	{ this->Sender(s); }

	virtual void
	operator()(Network::PacketListPtr& pktList)
	{ this->Received(pktList); }

    protected:
	typedef std::map<int, SingleEvent<Network::PacketListPtr&> >
	ReceiverMap;

	IPv4::Node*
	Sender_;
	
	ReceiverMap
	Receivers_;
	    
	void
	Received(Network::PacketListPtr& pkt);
	
	virtual void
	Drop(Network::PacketListPtr& pkt){}
    };
}

// std::ostream&
// operator<<(std::ostream& o, const Nodes::UDPNode& n);

#endif // UDP__NODE_HXX__
