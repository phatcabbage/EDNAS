#ifndef NETWORK__LINKEVENT_HXX__
#define NETWORK__LINKEVENT_HXX__

#include "Protocols/Ethernet.h++"

namespace Network
{
    struct LinkHandle;

    using Protocols::Ethernet;

    struct LinkEvent
    {
	LinkHandle& Handle_;
	FramePtr Frame_;
	
	LinkEvent(LinkHandle& h, Ethernet::PacketPtr& f)
	    : Handle_(h)
	    , Frame_(f)
	{}

	virtual void
	operator()() = 0;
	
	virtual ~LinkEvent(){}
    };
    struct
    FrameReceived : public LinkEvent
    {
	FrameReceived(LinkHandle& h, FramePtr& f);
	virtual void operator()();
    };
}

#endif // NETWORK__LINKEVENT_HXX__
