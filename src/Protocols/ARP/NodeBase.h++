#ifndef ARP__NODEBASE_HXX__
#define ARP__NODEBASE_HXX__

#include "Simulation/Event.h++"
#include "Protocol.h++"
#include "Packet.h++"

namespace Ethernet
{
    struct Node;
}

namespace IPv4
{
    struct NodeBase; 
}

namespace ARP
{
    struct NodeBase : public EventHandler<Network::PacketListPtr&>
    {
	struct ResolutionArg
	{
	    enum StatusCode
	    {
		INVALID,
		UNKNOWN,
		TIMEDOUT,
		VALID
	    };
	    StatusCode Status;
	    MACAddress MAC;
	    IPAddress IP;
	};

	virtual Event<ResolutionArg&>&
	OnResolved() = 0;

	virtual void
	Flush() = 0;

	virtual void
	Flush(IPAddress ip) = 0;
	
	virtual bool
	Known(IPAddress ip) const = 0;

	virtual ResolutionArg
	Resolve(IPAddress ip) const = 0;
	
	virtual void
	Request(IPAddress ip) = 0;

	IPAddress
	Address() const
	{ return this->IPv4Address(); }
	
	MACAddress
	EthernetAddress() const;

	IPAddress
	IPv4Address() const;

	Ethernet::Node*
	EthernetNode() const
	{ return this->EtherNode_; }

	IPv4::NodeBase*
	IPNode() const
	{ return this->IPNode_; }

	void
	EthernetNode(Ethernet::Node* hw);

	void
	IPv4Node(IPv4::NodeBase* n)
	{ this->IPNode_ = n; }

	virtual void
	operator()(Network::PacketListPtr& pktList)
	{ this->Received(pktList); }

	virtual void
	Received(Network::PacketListPtr& pktList);
    
	TimeType
	Timeout() const
	{ return this->Timeout_; }

	void
	Timeout(TimeType timeout)
	{ this->Timeout_ = timeout; }

	int
	Timeouts() const
	{ return this->Timeouts_; }
	
	void
	Timeouts(int t)
	{ this->Timeouts_ = t; }

	virtual 
	~NodeBase()
	{}

    protected:

	Ethernet::Node* EtherNode_;
	IPv4::NodeBase* IPNode_;

	TimeType Timeout_;
	int      Timeouts_;
    
	NodeBase(Ethernet::Node* eth = NULL,
	     IPv4::NodeBase* ip = NULL)	
	    : EventHandler<Network::PacketListPtr&>()
	    , EtherNode_(NULL)
	    , IPNode_(ip)
	    , Timeout_(Protocol::TIMEOUT)
	    , Timeouts_(Protocol::TIMEOUTS)
	{ this->EthernetNode(eth); }

	virtual void
	UpdateTable(IPAddress ip, MACAddress mac, ResolutionArg::StatusCode) = 0;
    };
}

#endif // ARP__NODEBASE_HXX__
