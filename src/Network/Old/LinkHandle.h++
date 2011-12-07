#ifndef NETWORK__LINKHANDLE_HXX__
#define NETWORK__LINKHANDLE_HXX__

#include "Protocols/Ethernet.h++"

#include "./Link.h++"

namespace Network
{
    struct NIC;
    using Protocols::Ethernet;
    struct LinkHandle
    {
	virtual double Delay() const;
	
	virtual double Upstream  () const = 0;
	virtual double Downstream() const = 0;

	virtual void Upstream  (double upstream)   = 0;
	virtual void Downstream(double downstream) = 0;
	virtual void Send(Ethernet::PacketPtr& f) = 0;

	LinkHandle(Link& link, NIC& nic)
	    : Link_(link)
	    , NIC_(nic)
	{}
	    
	virtual ~LinkHandle(){}

    protected:
	Link& Link_;
	NIC& NIC_;

	LinkHandle(const LinkHandle&);

	void 
	Received(Ethernet::PacketPtr& f);
	
	friend class Link;
	friend class FrameReceived;
    };

    template<Link::Side S>
    struct SideLinkHandle : public LinkHandle
    {
	double Upstream() const
	{ return Link_.Upstream<S>();}
	
	double Downstream() const
	{ return Link_.Downstream<S>(); }
	
	void Upstream(double upstream)
	{ Link_.Upstream<S>(upstream); }
	
	void Downstream(double downstream)
	{ Link_.Downstream<S>(downstream); }
	
	void Send(Ethernet::PacketPtr& f)
	{ Link_.Send<S>(f);}
	 
	SideLinkHandle(Link& link, NIC& nic)
	    : LinkHandle(link, nic)
	{}
	
	virtual ~SideLinkHandle(){}
    };

    template struct SideLinkHandle<Link::Left>;
    template struct SideLinkHandle<Link::Right>;
}

#endif // NETWORK__LINKHANDLE_HXX__
