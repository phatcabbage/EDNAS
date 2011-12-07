#ifndef ARP__NODE_HXX__
#define ARP__NODE_HXX__

#include <map>

#include "Simulation/Timer.h++"
#include "Simulation/Event.h++"
#include "NodeBase.h++"

namespace ARP
{
    struct Node : public NodeBase
    {
	enum { EXPIRATION_TIME = 600 };

    protected:
	std::map<IPAddress, ResolutionArg>
	ResolutionTable_;
	
	std::map<IPAddress, int>
	Pending_;

	MultiEvent<ResolutionArg&>
	OnResolved_;

    public:
	/* NodeBase Interface */
	virtual Event<ResolutionArg&>&
	OnResolved()
	{ return this->OnResolved_; }

	virtual void
	Flush();
	
	virtual void
	Flush(IPAddress ip);

	virtual bool
	Known(IPAddress ip) const;

	virtual ResolutionArg
	Resolve(IPAddress ip) const;

	virtual void
	Request(IPAddress ip);

	Node(Ethernet::Node* eth = NULL,
	     IPv4::NodeBase* ip  = NULL)
	    : NodeBase(eth, ip)
	    , ResolutionTable_()
	    , Pending_()
	{}
   
    protected:
	virtual void
	UpdateTable(IPAddress ip, MACAddress mac, ResolutionArg::StatusCode status);

	void
	Resolved(ResolutionArg& a)
	{ this->OnResolved_(a); }
	
	void
	TimedOut(IPAddress ip);

	void
	Expired(IPAddress ip);
    };
}

#endif // ARP__NODE_HXX__
