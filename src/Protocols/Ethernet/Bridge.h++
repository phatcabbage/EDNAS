#ifndef NETWORK__BRIDGE_HXX__
#define NETWORK__BRIDGE_HXX__

#include "SEDNAS.h++"
#include "Simulation/Event.h++"
#include "Protocol.h++"
#include "Packet.h++"
#include "Protocols/Templates/Node.h++"
#include "NodeBase.h++"

namespace Ethernet
{
    struct Node;
    struct Bridge
    {
    private:
	struct Port;
	
	enum Action
	{
	    FORWARDALL,
	    FORWARDONE,
	    DROP
	};

	typedef std::vector<Port*> PortList;
	typedef unsigned int PortNumber;

	typedef std::map<Protocol::AddressType, Action>
	RoutingPolicyMap;

	typedef std::map<Protocol::AddressType, PortNumber>
	RoutingTableType;

	static unsigned int G_ID;

	static TimeType DEF_LINK_DELAY;
	static SizeType DEF_LINK_BANDWIDTH;

	static bool 
	IsValidAddress(Protocol::AddressType mac);

	PortList
	Ports;

	RoutingPolicyMap
	RoutingPolicies;

	RoutingTableType
	RoutingTable;

	void
	CreateStaticRoutes();

	void
	UpdateRoute(Protocol::AddressType mac, PortNumber port);

	void
	ForwardAll(Network::PacketListPtr& f, PortNumber src);
	
	void
	ForwardOne(Network::PacketListPtr& f, PortNumber dest);
	
	void
	Drop(Network::PacketListPtr& f);

	void
	Received(Network::PacketListPtr& f, PortNumber src);

    public:
	const unsigned int ID;

	MultiEvent<Network::PacketListPtr&>
	OnReceived;

	static void
	SetDefaultLinkDelay(TimeType t);
	
	static void
	SetDefaultLinkBandwidth(SizeType s);

	Bridge();
	~Bridge();

	Bridge&
	operator+=(NodeBase& nic);
	
	Bridge&
	operator+=(Bridge& other);
     }; 

    struct Bridge::Port : public NodeBase
    {
	Bridge* Bridge_;
	Bridge::PortNumber PortNumber_;

	void
	Forward(Network::PacketListPtr& pktList)
	{ if (Sender_) Sender_->Send(pktList); }

        Port(Bridge& s, Bridge::PortNumber portnumber)
	    : NodeBase()
	    , Bridge_(&s)
	    , PortNumber_(portnumber)
	{}

	virtual ~Port (){}

    protected:
	void
	Received(Network::PacketListPtr& pktList)
	{ Bridge_->Received(pktList, PortNumber_); }
    };
}

#endif // NETWORK__BRIDGE_HXX__
