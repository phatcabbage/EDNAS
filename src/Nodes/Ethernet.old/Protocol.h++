#ifndef PROTOCOLS__ETHERNET_HXX__
#define PROTOCOLS__ETHERNET_HXX__

#include "SEDNAS.h++"
#include "Address.h++"

namespace Protocols
{
    struct Ethernet
    {
	enum SubType
	{
	    IPv4,
	    IPv6,
	    ARP,
	    RARP,
	    FAKE
	};
	
	struct Packet;
	typedef Packet PacketType;
	typedef std::shared_ptr<Packet> PacketPtr;

	typedef TAddress<Ethernet> AddressType;

	static const AddressType DEFAULT;
	static const AddressType BROADCAST;
	
	static AddressType G_MAC;
	
	static inline AddressType 
	AllocateAddress() { return ++G_MAC; }
    };
}

std::string
ToString(Protocols::Ethernet::AddressType);

std::ostream&
operator<<(std::ostream&, const Protocols::Ethernet::AddressType&);

std::string
ToString(Protocols::Ethernet::SubType);

#endif // PROTOCOLS__ETHERNET_HXX__
