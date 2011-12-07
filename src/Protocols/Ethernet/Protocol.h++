#ifndef ETHERNET__PROTOCOL_HXX__
#define ETHERNET__PROTOCOL_HXX__

#include "SEDNAS.h++"

#include "Protocols/Templates/Address.h++"
#include "Logging/OStreamLogger.h++"
#include "Protocols/Templates/Node.h++"
#include "Protocols/ProtocolList.h++"

namespace Links
{
    struct ILinkHandle;
};

namespace Ethernet
{
    struct Protocol;
    struct Packet;
    typedef TAddress<Protocol> Address;
    
    struct Protocol
    {
	typedef Protocol WrapperType;
	static const int Layer = 0;
	static const Protocols::ProtocolID ID = Protocols::ETHERNET;
	// enum WrappedType : Protocols::ProtocolID
	// {
	//     IPv4,
	//     IPv6,
	//     ARP,
	//     RARP,
	//     FAKE
	// };
	
	typedef Address		AddressType;
//	typedef WrappedType	SlotType;

	const static bool IsWrapper = true;

	static const AddressType INVALID_ADDRESS;
	static const AddressType DEFAULT_ADDRESS;
	static const AddressType BROADCAST_ADDRESS;
	
	static inline AddressType AllocateAddress() { return ++G_MAC; }

	static Logging::OStreamLogger Log;
    private:
	static AddressType G_MAC;
    };
}

std::ostream&
operator<<(std::ostream&, const Ethernet::Protocol::AddressType&);

#endif // ETHERNET__PROTOCOL_HXX__
