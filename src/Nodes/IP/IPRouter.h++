#ifndef NODES__IPROUTER_HXX__
#define NODES__IPROUTER_HXX__

#include <map>
#include <memory>
#include <vector>

#include "IPNode.h++"

namespace Nodes
{
    struct IPRouter
    {
	IPRouter();
	virtual ~IPRouter();
	
	size_t
	Count() const
	{ return Interfaces_.size(); }

	IPNode&
	AddInterface(const IPv4::AddressType ip = IPv4::DEFAULT,
	      const IPv4::Netmask netmask = IPv4::CLASS_C);

	IPNode&
	operator[](unsigned int index);

    protected:
	typedef std::shared_ptr<IPNode> IFace;

	std::vector<IFace>
	Interfaces_;
	
	std::map<IPv4::AddressType, IPNode*>
	RoutingTable_;
	
	EventAdapter<IPRouter, IPNode::AddressChangedEventArgs&>
	NodeAddressChangedAdapter_;
	
	EventAdapter<IPRouter, IPNode::MessageArg&>
	ReceivedAdapter_;
    };
}

#endif // NODES__IPROUTER_HXX__
