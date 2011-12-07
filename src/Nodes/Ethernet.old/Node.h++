#ifndef PROTOCOLS__ETHERNET__NODE_HXX__
#define PROTOCOLS__ETHERNET__NODE_HXX__

#include <map>
#include <memory>

#include "Logging/Logged.h++"
#include "Core/Signaling/SingleEvent.h++"
#include "Core/Signaling/EventAdapter.h++"

#include "Protocols/Ethernet.h++"

#include "Link/Link.h++"

namespace Nodes
{
    using Protocols::Ethernet;

    struct EthernetBase : 
	public Logging::Logged<EthernetBase>
    {
	Ethernet::AddressType
	Address() const
	{ return this->MAC_; }
	
	Links::ILinkHandle<Ethernet>*
	Handle() const
	{ return this->Handle_; }

	void
	Handle(Links::ILinkHandle<Ethernet>* handle)
	{ this->Handle_ = handle; }

	void
	Send(Ethernet::PacketPtr& pkt)
	{ 
	    if (Handle_)
	    {
		pkt->Sender = this->Address(); 
		this->Handle_->Send(pkt); 
	    }
	}

	template<typename _P>
	void
	Send(Ethernet::SubType type, _P& pkt, Ethernet::AddressType rcv)
	{
	    if (this->Handle_)
	    {
		Ethernet::PacketPtr ethpkt =
		    std::make_shared<Ethernet::PacketType>
		    (type, pkt, pkt->Size(), rcv, this->Address());

		this->Handle_->Send(ethpkt);
	    }
	}
	
	explicit
	EthernetBase()
	    : MAC_(Ethernet::AllocateAddress())
	    , Handle_()
	{}

	virtual 
	~EthernetBase()
	{}

	virtual void
	Received(Ethernet::PacketPtr& pkt) = 0;

    protected:
	EthernetBase(const EthernetBase&);

	Ethernet::AddressType
	MAC_;
	
	Links::ILinkHandle<Ethernet>*
	Handle_;
    };

    struct EthernetNode : public EthernetBase
    {
	typedef SingleEvent<Ethernet::PacketPtr&> MessageEvent;

	MessageEvent&
	operator[](Ethernet::SubType type)
	{ return this->Handlers[type]; }
	
	virtual void
	Received(Ethernet::PacketPtr& pkt)
	{ this->Handlers[pkt->Type](pkt); }

	EthernetNode() : EthernetBase(), Handlers() {}
    protected:
	std::map<Ethernet::SubType,MessageEvent>
	Handlers;
    };
}    

#endif // PROTOCOLS__ETHERNET__NODE_HXX__
