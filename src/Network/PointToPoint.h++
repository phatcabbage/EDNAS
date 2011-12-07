#ifndef LINKS__POINTTOPOINT_HXX__
#define LINKS__POINTTOPOINT_HXX__

#include "SEDNAS.h++"

#include "Simulation/Timer.h++"
#include "Protocols/Templates/Node.h++"
#include "Link.h++"

#include "Packet.h++"

namespace Network
{
    struct PointToPoint
    {
    public:
	static SizeType 
	MAX_BUFFER;

	struct Handle;

	enum Side : unsigned int
	{
	    Left,
	    Right,
	    COUNT
	};

	struct DropEventArg
	{
	    PointToPoint* Link_;
	    Side Side_;
	    PacketListPtr PktList_;
	    DropEventArg(PointToPoint* l, Side s, PacketListPtr p)
		: Link_(l), Side_(s), PktList_(p) {}
	};

	static
	MultiEvent<DropEventArg&>
	OnDrop;

    protected:
	static
	int
	G_ID; 

	const int
	ID;

	std::weak_ptr<PointToPoint>
	SelfPtr_;

	TimeType
	Delay_;

	SizeType
	Bandwidth_[COUNT];
	
	Handle*
	Handle_[COUNT];

	std::list<PacketListPtr>
	Queue_[COUNT];
	
	SizeType
	MaxBuffer_[COUNT];

	SizeType
	Buffer_[COUNT];

	inline static Side Other(Side s)
	{ return static_cast<Side>((s & 0xFFFFFFFF) ^ 1); }

	PointToPoint(const PointToPoint&);

	void TransferPacket(Side side);
	
	void Connected(Side side, Handle*);
	void Disconnected(Side side);

	PointToPoint(TimeType delay,
		     SizeType leftUp,
		     SizeType rightUp);
	    
    public:

	static
	std::shared_ptr<PointToPoint>
	Create(TimeType delay = 50 * Time::MILLISECOND,
	       SizeType leftUp = 1 * Bandwidth::MEGABIT,
	       SizeType rightUp = 1 * Bandwidth::MEGABIT);

	TimeType
	Delay() const
	{ return this->Delay_; }

	SizeType
	Upstream(Side side) const
	{ return this->Bandwidth_[side]; }

	SizeType
	Downstream(Side side) const
	{ return this->Bandwidth_[Other(side)]; }
	
	std::shared_ptr<Handle>
	SideHandle(Side side);

	std::shared_ptr<Handle>
	LeftHandle()
	{ return this->SideHandle(Left); }

	std::shared_ptr<Handle>
	RightHandle()
	{ return this->SideHandle(Right); }
	
	SizeType
	QueuedData(Side side) const
	{ return Buffer_[side]; }

	SizeType
	MaxQueue(Side side) const
	{ return MaxBuffer_[side]; }

	void
	MaxQueue(Side side, SizeType size)
	{ MaxBuffer_[side] = size; }

	virtual void 
	Send(Side side, PacketListPtr& pkt);

	void
	Sent(Side side);
	
	virtual void
	Drop(Side side, PacketListPtr& pkt);

	/* TEMPORARY!! */
	std::list<PacketListPtr>&
	Queue(Side side)
	{ return this->Queue_[side]; }

	virtual ~PointToPoint();
    };
}

#include "P2PHandle.h++"
#endif // LINKS__POINTTOPOINT_HXX__
