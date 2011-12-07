#include <cassert>
#include "Logging/OStreamLogger.h++"
#include "PointToPoint.h++"
#include "P2PHandle.h++"

#include "Simulation/Timer.h++"

namespace Network
{
    SizeType
    PointToPoint::MAX_BUFFER =
	    1 * Size::MEGABYTE;	

    MultiEvent<PointToPoint::DropEventArg&>
    PointToPoint::
    OnDrop;

    int
    PointToPoint::
    G_ID(0);

    void
    PointToPoint::
    Send(Side side, PacketListPtr& pkt)
    {
	auto pktSize = pkt->Size();
	if (Buffer_[side] + pktSize > MaxBuffer_[side])
	{
	    Drop(side, pkt);
	    return;
	}
	bool empty = Queue_[side].empty();
	Queue_[side].push_back(pkt);
	SizeType presize = Buffer_[side];
	Buffer_[side] += pktSize;
	assert(Buffer_[side] == presize + pktSize);


	if (empty)
	    TransferPacket(side);
    }

    std::shared_ptr<PointToPoint::Handle>
    PointToPoint::
    SideHandle(Side side)
    {
	if (!Handle_[side])
	{
	    new Handle(*this, side);
	    return std::shared_ptr<Handle>(Handle_[side]);
	}
	else
	{
	    return std::shared_ptr<PointToPoint::Handle>();
	}
    }

    void
    PointToPoint::
    Sent(Side side)
    {
	PacketListPtr pkt = Queue_[side].front();
	Queue_[side].pop_front();
	Buffer_[side] -= pkt->Size();
	assert(Buffer_[side] >= 0);
	// P2PLog << "[" << ID << "] Packet sent.\n"
	//        << "\tSide: " << side << "\n"
	//        << "\tPacket: " << pkt->Size() / Size::BYTE << " B\n"
	//        << "\tBuffer: " << presize/Size::BYTE << "B prev /"
	//        << " " << Buffer_[side] / Size::BYTE << "B current /"
	//        << " " << MaxBuffer_[side] / Size::BYTE << "B max"
	//        << Logging::endl;


	if (!Queue_[side].empty())
	    TransferPacket(side);

	Handle* handle = Handle_[Other(side)];
	if (handle) handle->Received(pkt);
    }

    void
    PointToPoint::
    TransferPacket(Side side)
    {
	SizeType size = Queue_[side].front()->Size();

	double dsize = size;
	double dband = Bandwidth_[side];
	double dtime = (dsize * Time::SECOND) / dband;

	TimeType transfertime = 
	    dtime;
	
	Timer::Main.Schedule(Delay_ + transfertime, 
			     *this, &PointToPoint::Sent,
			     side);
    }

    PointToPoint::
    PointToPoint(TimeType delay,
		 SizeType leftUp,
		 SizeType rightUp)
	: ID(++G_ID)
	, SelfPtr_()
	, Delay_(delay)
	, Bandwidth_({ leftUp, rightUp })
	, Handle_({ NULL, NULL})
	, Queue_()
	, MaxBuffer_({ MAX_BUFFER, MAX_BUFFER })
	, Buffer_({0,0})
    { }

    void
    PointToPoint::
    Connected(Side side, Handle* handle)
    {
	assert(!Handle_[side]);
	Handle_[side] = handle;
    }

    void
    PointToPoint::
    Disconnected(Side side)
    {
	assert(Handle_[side]);
	Handle_[side] = NULL;
    }

    void
    PointToPoint::
    Drop(Side side, PacketListPtr& pkt)
    {
	// P2PLog << "[" << ID <<"] Dropping packet of size " << pkt->Size() << "\n"
	//        << "\t" << Buffer_[side] << " of " << MaxBuffer_[side]
	//        << " bits buffered\n" 
	//        << "\tas " << Queue_[side].size() 
	//        << " packets" << Logging::endl;
	DropEventArg dea(this, side, pkt);
	OnDrop(dea);
    }

    PointToPoint::
    ~PointToPoint()
    {}

    std::shared_ptr<PointToPoint>
    PointToPoint::
    Create(TimeType delay,
	   SizeType leftUp,
	   SizeType rightUp)
    {
	std::shared_ptr<PointToPoint> link(new PointToPoint(delay, leftUp, rightUp));
	link->SelfPtr_ = link;
	return link;
    }

    PointToPoint::Handle::
    Handle(PointToPoint& l, Side s)
	: Link(l.SelfPtr_)
	, LinkSide(s)
	, OnReceive_()
    {
	Link->Connected(LinkSide, this);
    }

    PointToPoint::Handle::
    ~Handle()
    {
	Link->Disconnected(LinkSide);
    }
}
