#ifndef NETWORK__IPNODE_HXX__
#define NETWORK__IPNODE_HXX__

#include <iosfwd>
#include <list>
#include <map>

#include "Core/Signaling/SingleEvent.h++"
#include "Logging/Logged.h++"

#include "Network/Ethernet.h++"
#include "Network/IPv4.h++"

#include "./NIC.h++"
#include "./Node.h++"

namespace Network
{
    struct IPNode : public EventHandler<Frame&>,
		    public Logging::Logged<IPNode>
    {
	typedef EventHandler<IPPacket&> Handler;
	typedef std::map<IPv4::SubType, SingleEvent<IPPacket&> >
	PacketHandlerMapType;

	typedef std::map<IPAddress, MACAddress>
	MACByIPMapType;

	const IPAddress
	IP;
	
	PacketHandlerMapType
	PacketHandlers;

	IPNode();
	~IPNode(){}

	IPAddress
	Gateway() const
	{ return this->Gateway_;}

	void
	Gateway(IPAddress a)
	{ this->Gateway_ = a; }

	Node*
	EtherNode() const
	{ return this->Node_; }

	void
	EtherNode(Node* node);

	void
	Send(IPPacket* pkt);

    protected:
	static unsigned int G_IP;

	MACByIPMapType
	MACByIP_;

	IPAddress
	Gateway_;

	Node*
	Node_;

	void
	ReceiveARPRequest(IPPacket* pkt);
	
	void
	ReceiveARPResponse(IPPacket* pkt);
	
	void
	Received(Frame& f);
	
	void
	operator()(Frame& f)
	{ this->Received(f); }
    };	

    std::ostream&
    operator&(std::ostream& o, const IPNode& n);
}

#endif // NETWORK__IPNODE_HXX__
