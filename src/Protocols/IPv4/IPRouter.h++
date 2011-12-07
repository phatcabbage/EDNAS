#ifndef IPV4__IPROUTER_HXX__
#define IPV4__IPROUTER_HXX__

//#include "SEDNAS.h++"
#include "Protocol.h++"

#include "Network/Packet.h++"

namespace IPv4
{
    struct NodeBase;
    struct IPRouter 
    {
	struct Interface;
	typedef int InterfaceNumber;

	virtual void
	Received(Network::PacketListPtr& pktList, InterfaceNumber n);

	InterfaceNumber
	AddInterface(Protocol::AddressType ip = Protocol::DEFAULT_ADDRESS,
		     Protocol::Netmask nm = Protocol::CLASS_C,
		     Protocol::AddressType gw = Protocol::INVALID_ADDRESS);

	Interface&
	operator[](InterfaceNumber i);

	IPRouter();
	virtual ~IPRouter();

	void
	AddRoute(Protocol::AddressType subnet, InterfaceNumber i);

    protected:
	std::vector<Interface*>
	Interfaces_;
	
	std::map<Protocol::AddressType, InterfaceNumber>
	RoutingTable_;
	
	virtual void
	Forward(Network::PacketListPtr& pktList, InterfaceNumber from);

	virtual void
	Drop(Network::PacketListPtr& pktList);

	virtual void
	InterfaceAddressChanged(InterfaceNumber i);

	virtual void
	Undeliverable(Network::PacketListPtr& pktList);

	static inline bool
	IsValid(Protocol::AddressType ip)
	{ 
	    return (ip != Protocol::INVALID_ADDRESS && 
		    ip != Protocol::DEFAULT_ADDRESS);
	}
	
    };
	
}

#endif // IPV4__IPROUTER_HXX__
