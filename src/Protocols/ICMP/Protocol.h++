#ifndef PROTOCOLS__ICMP_HXX__
#define PROTOCOLS__ICMP_HXX__

#include "SEDNAS.h++"
#include "Protocols/Address.h++"
#include "Protocols/IPv4/Protocol.h++"
#include "IPv4.h++"

namespace ICMP
{
    struct Protocol;
    struct Packet;


    struct Protocol
    {
	typedef IPv4::Protocol WrapperType;
	
	static const WrapperType::WrappedType TypeCode = IPv4::Protocol::ICMP;
	
	enum WrappedType
	{
	    EchoRequest,
	    EchoReply,
	    DestinationUnreachable,
	    Redirect,
	    SourceQuench,
	    TimeExceeded
	};

	typedef IPv4::Protocol::AddressType AddressType;

	typedef Packet PacketType;
	typedef std::shared_ptr<PacketType> PacketPtr;
    };

    struct ICMP::Packet
    {
	static const SizeType HEADER_SIZE;

	static SizeType
	Size() 
	{ return HEADER_SIZE; }

	ICMP::SubType
	Type;
    };
}

#endif // PROTOCOLS__ICMP_HXX__
