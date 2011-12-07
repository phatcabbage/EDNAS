#ifndef UDP__PROTOCOL_HXX__
#define UDP__PROTOCOL_HXX__

#include "SEDNAS.h++"
#include "Protocols/Templates/Address.h++"
#include "Protocols/IPv4.h++"

#include "Protocols/ProtocolList.h++"

namespace UDP
{
    struct Packet;
    struct Node;
    struct Protocol
    {
	typedef IPv4::Protocol WrapperType;
	static const Protocols::ProtocolID ID = Protocols::UDP;
	static const int Layer = IPv4::Protocol::Layer;

	typedef IPv4::Protocol::AddressType AddressType;
	typedef Packet PacketType;
    };
}

#endif // PROTOCOLS__UDP_HXX__
