#ifndef NETWORK__IPV4_HXX__
#define NETWORK__IPV4_HXX__

#include "./Ethernet.h++"

namespace Network
{
    struct IPv4
    {
	typedef Ethernet ContainerProtocol;
	static const ContainerProtocol::SubType TypeCode = Ethernet::IPv4;
	enum SubType
	{
	    ICMP,
	    IGMP,
	    TCP,
	    UDP,
	    OSPF,
	    SCTP
	};
	typedef Packet<IPv4> PacketType;
	typedef Address<IPv4,unsigned int> AddressType;
    };

    template<> struct PacketSize<IPv4> { enum { SIZE = 160 }; };
    
    typedef IPv4::AddressType IPAddress;
    typedef IPv4::PacketType IPPacket;
};

#endif // NETWORK__IPV4_HXX__
