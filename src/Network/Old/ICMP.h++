#ifndef NETWORK__ICMP_HXX__
#define NETWORK__ICMP_HXX__

#include "./IPv4.h++"

namespace Network
{
    struct ICMP
    {
	typedef IPv4 ContainerProtocol;
	static const IPv4::SubType TypeCode = IPv4::ICMP;
	enum SubType
	{
	    EchoRequest,
	    EchoReply,
	    DestinationUnreachable,
	    SourceQuench,
	    TimeExpired
	};
	typedef Packet<ICMP> PacketType;
	typedef Address<ICMP,void> AddressType;
    };

    template<> struct PacketSize<ICMP> { enum { SIZE = 32 }; };
    
    template<ICMP::SubType _T>
    struct ICMPMsg
    {
	typedef ICMP ContainerProtocol;
	static const ICMP::SubType TypeCode = _T;
	typedef Packet<ICMPMsg> PacketType;
    };

    template<ICMP::SubType _T>
    struct Packet<ICMPMsg<_T>> : public Packet<ICMP>
    {
	Packet() : Packet<ICMP>(_T,0){}
    };

    typedef Packet<ICMP> ICMPPacket;
    typedef ICMPMsg<ICMP::EchoRequest> ICMPEchoRequest;
    typedef ICMPMsg<ICMP::EchoReply>   ICMPEchoReply;
    typedef Packet<ICMPEchoRequest>    ICMPEchoRequestPacket;
    typedef Packet<ICMPEchoReply>      ICMPEchoReplyPacket; 
}

#endif // NETWORK__ICMP_HXX__
