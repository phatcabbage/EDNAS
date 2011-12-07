#ifndef LINKS__LINK_HXX__
#define LINKS__LINK_HXX__

#include "SEDNAS.h++"
#include "Simulation/Event.h++"
#include "Packet.h++"
#include "Protocols/Templates/Node.h++"

namespace Network
{
    struct ILinkHandle
    {
	virtual void
	Send(PacketListPtr& pkt) = 0;

	virtual TimeType
	Delay() const = 0;

	virtual SizeType
	Upstream() const = 0;

	virtual SizeType
	Downstream() const = 0;
	
	virtual Event<PacketListPtr&>&
	OnReceive() = 0;

	virtual SizeType
	QueuedData() const = 0;
	
	virtual ~ILinkHandle(){}
    };
}

#endif // NETWORK__LINK_HXX__
