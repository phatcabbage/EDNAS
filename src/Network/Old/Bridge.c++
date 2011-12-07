#include <utility>
#include "./Frame.h++"
#include "./Switch.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Network::Switch>::
    Logger("Switch");
}

namespace Network
{
    unsigned int
    Switch::
    G_ID(0);

    Switch::
    Switch()
	: NICHandlers()
	, RoutingPolicies()
	, RoutingTable()
	, ID(++G_ID)
    {
	Logger << "Created #" << ID << Logging::endl;
    }
    
    void
    Switch::
    ForwardAll(Frame& f, NIC& sourceNic)
    {
	for (NICHandlerMapType::iterator 
		 it = NICHandlers.begin(), 
		 end = NICHandlers.end();
	     it != end; ++it)
	    if (it->second.NIC_ != &sourceNic)
		it->second.Send(f);
    }	

    void
    Switch::
    ForwardOne(Frame& f, NIC& nic)
    {
	nic.Send(&f);
    }

    void
    Switch::
    Drop(Frame& f)
    {}

    Switch&
    Switch::
    operator+=(NIC& nic)
    {
	if (this->NICHandlers.count(&nic) != 0) return *this;
	NICHandler nh(*this, nic);
	std::pair<NICHandlerMapType::iterator,bool> nhit = 
	    this->NICHandlers
	    .insert(std::make_pair(&nic, nh));
	nic.OnReceived = nhit.first->second;
	this->RoutingPolicies[nic.MAC()] = DROP;
	return *this;
    }

    Switch&
    Switch::
    operator-=(NIC& nic)
    {
	this->NICHandlers.erase(&nic);
	return *this;
    }

    void
    Switch::
    Received(Frame& f, NIC& nic)
    {
	if (IsLocalhost(f.Sender)) // no infinite loops
	{
	    Drop(f);
	    return;
	}

	if (!RoutingTable.count(f.Sender) && f.Sender != 0)
	{
	    RoutingTable[f.Sender] = &nic;
	    if (RoutingPolicies[f.Sender] != DROP)
		RoutingPolicies[f.Sender] = FORWARDONE;
	}

	Action a = RoutingPolicies[f.Receiver];
	switch(a)
	{
	case (FORWARDALL):
	    ForwardAll(f, nic);
	    break;
	case (FORWARDONE):
	    ForwardOne(f, *(RoutingTable[f.Receiver]));
	    break;
	default:
	    Drop(f);
	    break;
	}
    }
    
    bool
    Switch::
    IsLocalhost(MACAddress ma) const
    {
	for(NICHandlerMapType::const_iterator 
		i = NICHandlers.begin(), 
		e = NICHandlers.end(); i != e; ++i)
	{
	    NIC* n = i->first;
	    MACAddress m = n->MAC();
	    if (m == ma) return true;
	}
	return false;
    }

    Switch::NICHandler::
    ~NICHandler()
    {
	if (this->NIC_) 
	    NIC_->OnReceived = NULL;
    }
}
