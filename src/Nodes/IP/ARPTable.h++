#ifndef NODES__ARPTABLE_HXX__
#define NODES__ARPTABLE_HXX__

#include <map>
#include "Protocols/Ethernet.h++"
#include "Protocols/IPv4.h++"

namespace Nodes
{
    using Protocols::Ethernet;
    using Protocols::IPv4;
    struct ARPTable
    {
	bool Update(IPv4::AddressType ip, Ethernet::AddressType mac)
	{ return Map_.insert(std::make_pair(ip,mac)).second; }
	bool Resolve(IPv4::AddressType ip, 
		     Ethernet::AddressType& mac) const
	{ 
	    auto it = this->Map_.find(ip);
	    if (it == Map_.end()) return false;
	    mac = it->second;
	    return true;
	}
	
	ARPTable() : Map_() {}
	
    private:
	std::map<IPv4::AddressType,Ethernet::AddressType> Map_;
    };
}

#endif // NODES__ARPTABLE_HXX__
