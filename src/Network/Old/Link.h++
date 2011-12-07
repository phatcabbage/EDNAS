#ifndef NETWORK__LINK_HXX__
#define NETWORK__LINK_HXX__

#include <list>

#include "SEDNAS.h++"

#include "Logging/Logged.h++"
#include "Protocols/Ethernet.h++"

namespace Network
{
    struct NIC;
    struct LinkHandle;

    using Protocols::Ethernet;

    struct Link : Logging::Logged<Link>
    {
	enum Side
	{
	    Left,
	    Right,
	    COUNT
	};

	TimeType
	Delay() const
	{ return this->Delay_; }

	template<Side S>
	SizeType
	Upstream() const
	{ return this->Bandwidth_[S]; } 
	
	template<Side S>
	SizeType
	Downstream() const
	{ return this->Bandwidth_[Other(S)]; }
	
	double
	Reliability() const
	{ return this->Reliability_; }

	void
	Delay(TimeType delay)
	{ this->Delay_ = delay; }
	
	template<Side S>
	void
	Upstream(SizeType upstream)
	{ this->Bandwidth_[S] = upstream; }
	
	template<Side S>
	void
	Downstream(SizeType downstream)
	{ this->Bandwidth_[Other(S)] = downstream; }

	void
	Reliability(double rel)
	{ this->Reliability_ = rel; }

	virtual ~Link();

	Link(NIC& left,
	     NIC& right,
	     TimeType delay = 0.05,
	     SizeType leftUp = 65536,
	     SizeType rightUp = 65536,
	     double reliability = 1.0);

    protected:
	Link(const Link& other);

	static inline 
	Side Other(const Side& s)
	{ return s == Left ? Right : Left; }
	
	LinkHandle*
	Handle_[COUNT];

	std::list<Ethernet::PacketPtr>
	Queue_[COUNT];
	
	TimeType
	Delay_;
	
	SizeType
	Bandwidth_[COUNT];

	double
	Reliability_;

	void
	Sent(Side s);

	struct SendTimer
	{
	    Link& L;
	    Side  S;

	    void
	    operator()()
	    { L.Sent(S); delete this; }
	    
	    SendTimer(Link& l, Side s);
	};

    public:
	template<Side S>
	void
	Send(Ethernet::PacketPtr& p);

	struct SendTimer;
    };

    struct LinkManager
    {
	static LinkManager
	Main;

	std::vector<Link*>
	Links;

	LinkManager&
	operator+=(Link* l)
	{
	    Links.push_back(l);
	    return *this;
	}

	LinkManager()
	    : Links() 
	{}

	~LinkManager();

	friend class Link;
    };	
}

#endif // NETWORK__LINK_HXX__
