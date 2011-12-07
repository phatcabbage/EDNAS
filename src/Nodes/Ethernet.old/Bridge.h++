#ifndef NETWORK__BRIDGE_HXX__
#define NETWORK__BRIDGE_HXX__

#include "SEDNAS.h++"
#include "Logging/Logged.h++"
#include "Simulation/Event.h++"
#include "Protocol.h++"
#include "Node.h++"

namespace Ethernet
{
    struct Bridge : Logging::Logged<Bridge>
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

	PortList
	Ports;

	RoutingPolicyMap
	RoutingPolicies;

	RoutingTableType
	RoutingTable;

	void
	CreateStaticRoutes();

	void
	ForwardAll(Protocol::PacketPtr& f, PortNumber src);
	
	void
	ForwardOne(Protocol::PacketPtr& f, PortNumber dest);
	
	void
	Drop(Protocol::PacketPtr& f);

	void
	Received(Protocol::PacketPtr& f, PortNumber src);

    public:
	const unsigned int ID;
	
	Bridge();
	~Bridge();

	Bridge&
	operator+=(
	
	Bridge&
	operator+=(Bridge& other);
     }; 

    struct Bridge::Port : public EthernetBase
    {
	Bridge* Bridge_;
	Bridge::PortNumber PortNumber_;

        Port(Bridge& s, Bridge::PortNumber portnumber)
	    : EthernetBase()
	    , Bridge_(&s)
	    , PortNumber_(portnumber)
	{}

	virtual ~Port (){}

	void
	Received(Ethernet::PacketPtr& pkt)
	{ Bridge_->Received(pkt, PortNumber_); }
    };
}

#endif // NETWORK__BRIDGE_HXX__
