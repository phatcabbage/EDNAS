#ifndef IPV4__PROTOCOL_HXX__
#define IPV4__PROTOCOL_HXX__

#include "SEDNAS.h++"
#include "Protocols/Templates/Address.h++"
#include "Protocols/Ethernet/Protocol.h++"
#include "Protocols/Templates/Node.h++"
#include "Logging/OStreamLogger.h++"

#include "Protocols/ProtocolList.h++"

namespace IPv4
{
    struct Protocol;
    struct Packet;
    typedef TAddress<Protocol> AddressType;
    
    struct Protocol
    {
	typedef Ethernet::Protocol WrapperType;
	static const Protocols::ProtocolID ID = Protocols::IPv4;
	static const int Layer = Ethernet::Protocol::Layer + 1;
	// enum WrappedType
	// {
	//     ICMP,
	//     IGMP,
	//     TCP,
	//     UDP,
	//     OSPF,
	//     SCTP
	// };

	typedef Packet		PacketType;
	typedef IPv4::AddressType	AddressType;

	typedef AddressType Netmask;

	// static std::string
	// SubTypeName(WrappedType w);

	static Logging::OStreamLogger
	Log;

	// Predefined Addresses
	static const AddressType BROADCAST_ADDRESS;//	= 0xFFFFFFFF; /* 255.255.255.255 */
	static const AddressType LOCALHOST_ADDRESS;//	= 0x7F000001; /* 127.  0.  0.  1 */
	static const AddressType DEFAULT_ADDRESS;//	= 0x00000000;  /*   0.  0.  0.  0 */
	static const AddressType INVALID_ADDRESS;//     = 0x00000000;

	// Network class netmasks
	static const AddressType CLASS_C;//	= 0xFFFFFF00; /* 255.255.255.  0 */
	static const AddressType CLASS_B;//	= 0xFFFF0000; /* 255.255.  0.  0 */
	static const AddressType CLASS_A;//	= 0xFF000000; /* 255.  0.  0.  0 */
    };
}

std::ostream&
operator<<(std::ostream&, const IPv4::Protocol::AddressType&);

// IPv4::Address
// StringToAddress(const std::string&);

// IPv4::Address
// StringToAddress(const char*);

#endif // IPV4__PROTOCOL_HXX__
