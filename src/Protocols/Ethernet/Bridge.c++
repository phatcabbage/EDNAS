#include <utility>
#include "Network/PointToPoint.h++"
#include "Bridge.h++"

// namespace Logging
// {
//     template<>
//     OStreamLogger
//     Logged<Ethernet::Bridge>::
//     Logger("Bridge");
// }

namespace Ethernet
{
    unsigned int
    Bridge::
    G_ID(0);

    TimeType
    Bridge::
    DEF_LINK_DELAY = 30 * Time::MILLISECOND;

    SizeType
    Bridge::
    DEF_LINK_BANDWIDTH = 128 * Size::KILOBYTE;

    void
    Bridge::
    SetDefaultLinkDelay(TimeType t)
    { Bridge::DEF_LINK_DELAY = t; }

    void
    Bridge::
    SetDefaultLinkBandwidth(SizeType s)
    { Bridge::DEF_LINK_BANDWIDTH = s; }

    Bridge::
    Bridge()
	: Ports()
	, RoutingPolicies()
	, RoutingTable()
	, ID(++G_ID)
	, OnReceived()
    {	this->CreateStaticRoutes();    }
    
    Bridge::
    ~Bridge()
    {
	// auto linklist = G_CreatedLinks[this];
	// for(auto it = linklist.begin(), end = linklist.end();
	//     it != end; ++it)
	//     delete *it;
	// G_CreatedLinks.erase(this);
	// for(auto it = Ports.begin(), end = Ports.end();
	//     it != end; ++it)
	//     delete *it;
    }

    bool
    Bridge::
    IsValidAddress(Protocol::AddressType mac)
    { return (mac != Protocol::INVALID_ADDRESS); }

    void
    Bridge::
    CreateStaticRoutes()
    {
	this->RoutingPolicies[Protocol::BROADCAST_ADDRESS] = FORWARDALL;
    }

    void
    Bridge::
    UpdateRoute(Protocol::AddressType mac, PortNumber port)
    {
	PortNumber& rtEntry = RoutingTable[mac];
	rtEntry = port;
	Action& rpEntry = RoutingPolicies[mac];
	if (rpEntry != DROP)
	    rpEntry = FORWARDONE;
    }

    void
    Bridge::
    ForwardAll(Network::PacketListPtr& f, PortNumber src)
    {
	bool orig = true; // send original packet on one port
	for (PortNumber i = 0, count = Ports.size(); i != count; ++i)
	{
	    if (i != src)
	    {
		if (orig)
		{
		    ForwardOne(f, i);
		    orig = false;
		}
		else 
		{
		    Network::PacketListPtr copy
			(new Network::PacketList(*(f.get())));
		    ForwardOne(copy, i);
		}
	    }
	}
    }	

    void
    Bridge::
    ForwardOne(Network::PacketListPtr& f, PortNumber dest)
    {
	Ports[dest]->Forward(f);
    }

    void
    Bridge::
    Drop(Network::PacketListPtr& f)
    {
//	Logger << "Dropping." << Logging::endl;
    }

    Bridge&
    Bridge::
    operator+=(NodeBase& node)
    {
	auto local = new Port(*this, Ports.size());
	Ports.push_back(local);
	auto link = 
	    Network::PointToPoint::Create
	    (DEF_LINK_DELAY, DEF_LINK_BANDWIDTH,
	     DEF_LINK_BANDWIDTH);
	local->Sender(link->LeftHandle());
	node.Sender(link->RightHandle());
	this->RoutingPolicies[local->Address()] = DROP;
	return *this;
    }

    Bridge&
    Bridge::
    operator+=(Bridge& other)
    {
	auto local = new Port(*this, Ports.size());
	Ports.push_back(local);
	other += *local;
	this->RoutingPolicies[local->Address()] = DROP;
	return *this;
    }

    void
    Bridge::
    Received(Network::PacketListPtr& pktList, PortNumber src)
    {
	this->OnReceived(pktList);
	Network::PacketPtr& ethpkt = (*pktList)[Protocol::Layer];
	Protocol::AddressType receiver = ethpkt->Receiver;
	Protocol::AddressType sender = ethpkt->Sender;

	// Update routing tables
	UpdateRoute(sender, src);

	switch(RoutingPolicies[receiver])
	{
	case (FORWARDALL):
	    // Logger << Logging::Debug << "\t" << "Forwarding on all ports."
	    // 	   << Logging::endl;
	    ForwardAll(pktList, src);
	    break;
	case (FORWARDONE):
	{
	    PortNumber pnum = RoutingTable[receiver];
	    // Logger << Logging::Debug << "\t" << "Forwarding on [Port " 
	    // 	   << pnum << "]" << Logging::endl;
	    ForwardOne(pktList, pnum);
	    break;
	}
	default:
	    Drop(pktList);
	    break;
	}
    }
}
