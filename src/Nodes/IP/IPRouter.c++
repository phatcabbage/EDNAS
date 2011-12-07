#include <algorithm>
#include "IPRouter.h++"

/*
namespace Nodes
{
    IPRouter::
    IPRouter()
	: Interfaces_()
	, RoutingTable_()
	, NodeAddressChangedAdapter_(*this, &IPRouter::NodeAddressChanged)
	, ReceivedAdapter_(*this, &IPRouter::Received)
    {}

    IPRouter::
    ~IPRouter()
    {}

    IPNode&
    AddInterface(IPv4::AddressType ip,
		 IPv4::Netmask netmask)
    {
	EthernetNode *ethnode = new EthernetNode;
	IFace iface = std::make_shared<IPNode>
	    (*ethnode, ip, netmask);
	iface->OnAddressChanged += this->NodeAddressChangedAdapter_;
	// REWORK 
	this->Interfaces_.push_back(iface);
	this->RoutingTable_[ip & netmask] = iface.get();
    }

    IPRouter&
    IPRouter::
    operator+=(IPNode& node)
    {
	if (!std::count(IPs.begin(), IPs.end(), &node))
	    this->IPs.push_back(&node);
	
	return *this;
    }
    
    IPRouter&
    IPRouter::
    operator+=(IPRouter& router)
    {
	IFace iface = std::make_shared<IPNode>();
	this->CreatedNodes_.push_back(iface);
	
    }
}
*/
