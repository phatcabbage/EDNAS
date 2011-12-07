#ifndef ARP__PROTOCOL_HXX__
#define ARP__PROTOCOL_HXX__

#include <memory>

#include "SEDNAS.h++"
#include "Protocols/Templates/Node.h++"

#include "Protocols/Ethernet/Protocol.h++"
#include "Protocols/IPv4/Protocol.h++"

#include "Protocols/ProtocolList.h++"

namespace ARP
{
    struct Packet;
    struct Protocol;

    typedef IPv4::Protocol::AddressType Address;

    struct Node;

    typedef IPv4::Protocol::AddressType
    IPAddress;

    typedef Ethernet::Protocol::AddressType
    MACAddress;

    struct Protocol
    {
	typedef Ethernet::Protocol WrapperType;

	static const int Layer = WrapperType::Layer + 1;

	static const Protocols::ProtocolID ID = Protocols::ARP;

	enum Message
	{
 	    Reply,
 	    Request
 	};

	typedef Address AddressType;
	typedef Packet   PacketType;

	static const AddressType DEFAULT_ADDRESS;
	static const AddressType INVALID_ADDRESS;
	
	static const TimeType    TIMEOUT;
	static const int         TIMEOUTS;
    };
}

#endif // ARP__PROTOCOL_HXX__
