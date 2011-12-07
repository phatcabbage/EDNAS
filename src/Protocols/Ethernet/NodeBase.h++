#ifndef ETHERNET__NODEBASE_HXX__
#define ETHERNET__NODEBASE_HXX__

#include "Simulation/Event.h++"
#include "Network/Packet.h++"
#include "Network/Link.h++"
#include "Protocol.h++"
#include "Packet.h++"

namespace Ethernet
{
    struct NodeBase : public EventHandler<Network::PacketListPtr&>
    {
	typedef std::shared_ptr<Network::ILinkHandle> SenderType;

	Protocol::AddressType
	Address() const
	{ return this->Address_; }

	SenderType
	Sender() const
	{ return this->Sender_; }

	void
	Sender(SenderType s)
	{
	    this->Sender_ = s;
	    if (s)
		s->OnReceive() += *this;
	    // if (this->Sender_) 
	    // {
	    // 	Event<Network::PacketListPtr&>& onrcv =
	    // 	    this->Sender_->OnReceive();
	    // 	onrcv -= *this;
	    // }
	    // this->Sender_ = s; 
	    // if (this->Sender_) 
	    // {
	    // 	Event<Network::PacketListPtr&>& onrcv =
	    // 	    this->Sender_->OnReceive();
	    // 	onrcv += *this;
	    // }
	}

	virtual void
	Send(Network::PacketListPtr& pktList,
	     const Protocol::AddressType& receiver)
	{
	    Packet::Wrap(pktList, receiver, this->Address_);
	    if (this->Sender_) 
		this->Sender_->Send(pktList);

	}

	virtual void
	operator()(Network::PacketListPtr& pktList)
	{ this->Received(pktList); }

    protected:
	Protocol::AddressType
	Address_;
	
	SenderType
	Sender_;

	NodeBase(SenderType s = SenderType())
	    : EventHandler<Network::PacketListPtr&>()
	    , Address_(Protocol::AllocateAddress())
	    , Sender_()
	{ this->Sender(s); }
	
	virtual ~NodeBase(){}

	virtual void
	Received(Network::PacketListPtr&) = 0;
    };
}

#endif // ETHERNET__NODEBASE_HXX__
