#ifndef NETWORK__ICMPNODE_HXX__
#define NETWORK__ICMPNODE_HXX__

#include <iosfwd>

#include "Core/Signaling/Event.h++"
#include "Logging/Logged.h++"

namespace Network
{
    typedef unsigned int IPAddress;
    struct IPNode;
    struct IPPacket;
    struct ICMPNode : public EventHandler<IPPacket&>,
		      public Logging::Logged<ICMPNode>
    {
	const unsigned int ID;

	ICMPNode();
	~ICMPNode();

	IPNode*
	IpNode() const
	{ return this->Node_; }

	void
	IpNode(IPNode* node);
	
	void
	SendPing(IPAddress a);

    protected:
	static const unsigned int G_ID;

	IPNode*
	Node_;
	
	void 
	Received(IPPacket& pkt);

	void
	operator()(IPPacket& pkt)
	{ this->Received(pkt);}

	void
	ReceivedPing(ICMPPacket& pkt);

	void
	SendPong(IPAddress a);

	void
	ReceivedPong(ICMPPacket& pkt);
    };

    std::ostream&
    operator<<(std::ostream& o, const ICMPNode& n);
}

#endif // NETWORK__ICMPNODE_HXX__
