#ifndef LINKS__P2PHANDLE_HXX__
#define LINKS__P2PHANDLE_HXX__

#include "PointToPoint.h++"

namespace Network
{
    struct PointToPoint::Handle : public ILinkHandle
    {
	void
	Send(PacketListPtr& pktList)
	{ Link->Send(LinkSide, pktList); }

	virtual Event<PacketListPtr&>&
	OnReceive()
	{ return this->OnReceive_; }

	virtual void Received(PacketListPtr& pktList)
	{ this->OnReceive_(pktList); }
	
	Handle(PointToPoint& l, Side s);

	virtual ~Handle();

	TimeType
	Delay() const
	{ return Link->Delay_; }

	SizeType
	Upstream() const
	{ return Link->Upstream(LinkSide); }

	SizeType
	Downstream() const
	{ return Link->Downstream(LinkSide); }

	SizeType
	QueuedData() const
	{ return Link->QueuedData(LinkSide); }

    protected:
	std::shared_ptr<PointToPoint> Link;
	Side LinkSide;
	MultiEvent<PacketListPtr&> OnReceive_;
    };
}

#endif // LINKS__P2PHANDLE_HXX__
