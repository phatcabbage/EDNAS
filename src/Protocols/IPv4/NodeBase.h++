#ifndef IPV4__NODEBASE_HXX__
#define IPV4__NODEBASE_HXX__

#include <memory>

#include "Protocols/Ethernet.h++"

#include "Protocol.h++"
#include "Packet.h++"
#include "Protocols/ARP/Node.h++"

#include "Simulation/Timer.h++"

namespace IPv4
{
    struct NodeBase : public EventHandler<ARP::NodeBase::ResolutionArg&>
		    , public EventHandler<Network::PacketListPtr&>
    {
	virtual void
	Send(Network::PacketListPtr& pkt, Protocol::AddressType receiver, 
	     Protocol::AddressType sender);

	virtual void
	Send(Network::PacketListPtr& pkt, Protocol::AddressType receiver)
	{ this->Send(pkt, receiver, this->Address()); }

	Ethernet::Node*
	Sender() const
	{ return this->Sender_; }

	virtual void
	Sender(Ethernet::Node* s);
	
	virtual void
	Flush()
	{ this->ARPResolver_->Flush();	}
	
	Protocol::AddressType
	Address() const
	{ return this->Address_; }

	Protocol::Netmask
	Netmask() const
	{ return this->Netmask_; }

	Protocol::AddressType
	Gateway() const
	{ return this->Gateway_; }

	void
	Address(Protocol::AddressType ip)
	{
	    if (!IsLocalNet(ip))
		this->Flush();

	    this->Address_ = ip;
	}

	void
	Netmask(Protocol::Netmask nm)
	{
	    this->Netmask_ = nm;
	    this->Flush();
	    ARPResolver_->Flush();
	}

	void
	Gateway(Protocol::AddressType gw)
	{ this->Gateway_ = gw; }

	virtual void
	operator()(Network::PacketListPtr& pkt)
	{ this->Received(pkt); }

	virtual void
	operator()(ARP::NodeBase::ResolutionArg& ra)
	{ this->ARPResolved(ra); }

	virtual void
	ARPResolved(ARP::NodeBase::ResolutionArg& ra);

	NodeBase(Ethernet::Node* s = NULL,
		 Protocol::AddressType	ip = IPv4::Protocol::DEFAULT_ADDRESS,
		 Protocol::Netmask	nm = IPv4::Protocol::CLASS_C,
		 Protocol::AddressType	gw = IPv4::Protocol::DEFAULT_ADDRESS)
	    : Sender_(NULL)
	    , Address_(ip)
	    , Netmask_(nm)
	    , Gateway_(gw)
	    , ARPResolver_(new ARP::Node(s,this))
	    , AwaitingARP_()
	{
	    this->Sender(s);
	    ARPResolver_->OnResolved() += *this;
	}

	virtual ~NodeBase(){}

	virtual void
	Received(Network::PacketListPtr& pktList) = 0;

    protected:
	Ethernet::Node*
	Sender_;

	Protocol::AddressType
	Address_;

	Protocol::Netmask
	Netmask_;

	Protocol::AddressType
	Gateway_;
	
	std::unique_ptr<ARP::Node>
	ARPResolver_;
	
	std::map<Protocol::AddressType, std::list<Network::PacketListPtr> >
	AwaitingARP_;

	bool
	IsLocalHost(Protocol::AddressType ip) const
	{ return (ip == this->Address_ || ip == IPv4::Protocol::LOCALHOST_ADDRESS); }
	    
	void
	SelfReceived(Network::PacketListPtr pktList)
	{ this->Received(pktList); }

	bool
	IsLocalNet(Protocol::AddressType ip) const
	{ return ((ip & Netmask_) == (this->Address_ & Netmask_)); }

	static bool
	IsBroadcast(Protocol::AddressType ip)
	{ return (ip == Protocol::BROADCAST_ADDRESS); }

	virtual void
	Send(Network::PacketListPtr& pktList, Ethernet::Protocol::AddressType mac);

	void
	SendBroadcast(Network::PacketListPtr& pktList);

	void
	SendLocalHost(Network::PacketListPtr& pktList);
	
	void
	SendLocalNet(Network::PacketListPtr& pktList);
	
	void
	SendGateway(Network::PacketListPtr& pktList);

	virtual void
	Undeliverable(Network::PacketListPtr& pktList);
    };
}

std::ostream&
operator<<(std::ostream&,const IPv4::NodeBase&);

#endif // IPV4__NODEBASE_HXX__
