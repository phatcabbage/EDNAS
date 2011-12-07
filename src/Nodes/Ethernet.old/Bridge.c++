#include <utility>
#include "Link/PointToPoint.h++"
#include "./Bridge.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Nodes::Bridge>::
    Logger("Bridge");
}

namespace Nodes
{
    std::map<Bridge*,std::list<Links::PointToPointLink<Ethernet>*> >
    G_CreatedLinks;

    unsigned int
    Bridge::
    G_ID(0);

    Bridge::
    Bridge()
	: Ports()
	, RoutingPolicies()
	, RoutingTable()
	, ID(++G_ID)
    {
	this->CreateStaticRoutes();
	Logger << "Created #" << ID << Logging::endl;
    }
    
    Bridge::
    ~Bridge()
    {
	auto linklist = G_CreatedLinks[this];
	for(auto it = linklist.begin(), end = linklist.end();
	    it != end; ++it)
	    delete *it;
	G_CreatedLinks.erase(this);
	for(auto it = Ports.begin(), end = Ports.end();
	    it != end; ++it)
	    delete *it;
    }

    void
    Bridge::
    CreateStaticRoutes()
    {
	this->RoutingPolicies[Ethernet::BROADCAST] = FORWARDALL;
    }

    void
    Bridge::
    ForwardAll(Ethernet::PacketPtr& f, PortNumber src)
    {
	for (PortNumber i = 0, count = Ports.size(); i != count; ++i)
	    if (i != src)
		ForwardOne(f, i);
    }	

    void
    Bridge::
    ForwardOne(Ethernet::PacketPtr& f, PortNumber dest)
    {
	Ports[dest]->Send(f);
    }

    void
    Bridge::
    Drop(Ethernet::PacketPtr& f)
    {
	Logger << "Dropping." << Logging::endl;
    }

    Bridge&
    Bridge::
    operator+=(EthernetBase& remote)
    {
	auto local = new Port(*this, Ports.size());
	Ports.push_back(local);
	auto link = new Links::PointToPointLink<Ethernet>();
	link->Connect(*local, remote);
	G_CreatedLinks[this].push_back(link);
	this->RoutingPolicies[local->Address()] = DROP;
	Logger << "|" << ID << "| Connection created:" << Logging::endl
	       << "\t[Port " << local->PortNumber_ << "][MAC "
	       << local->Address() << "](local) -> [MAC " 
	       << remote.Address() << "](remote)" << Logging::endl;
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
    Received(Ethernet::PacketPtr& f, PortNumber src)
    {
	Logger << "|" << ID << "| Received Frame on [Port " << src << "]" << Logging::endl
	       << "\t[MAC " <<  f->Sender << "] -> [MAC " << f->Receiver << "]" << Logging::endl;
	if (f->Receiver == Ethernet::BROADCAST) // Special case: ARP Request
	{
	    ForwardAll(f, src);
	    return;
	}
	Logger << Logging::Debug << "|" << ID 
	       << "| Routing Table: " << Logging::endl;

	for(auto it = RoutingTable.begin(), end = RoutingTable.end(); it != end; ++it)
	    Logger << Logging::Debug << "\t[MAC " << it->first << "] := [Port " << it->second << "]" << Logging::endl;

	// Update routing tables
	RoutingTable[f->Sender] = src;
	Action& action = RoutingPolicies[f->Receiver];
	if (RoutingTable.count(f->Receiver))
	    action = FORWARDONE;
	switch(action)
	{
	case (FORWARDALL):
	    Logger << Logging::Debug << "\t" << "Forwarding on all ports."
		   << Logging::endl;
	    ForwardAll(f, src);
	    break;
	case (FORWARDONE):
	{
	    PortNumber pnum = RoutingTable[f->Receiver];
	    Logger << Logging::Debug << "\t" << "Forwarding on [Port " 
		   << pnum << "]" << Logging::endl;
	    ForwardOne(f, pnum);
	    break;
	}
	default:
	    Drop(f);
	    break;
	}
    }
}
