#ifndef NETWORK__ICMPPACKET_HXX__
#define NETWORK__ICMPPACKET_HXX__

#include "./IPPacket.h++"

namespace Network
{
    struct ICMPPacket : public IPPacket
    {
	enum SubType
	{
	    EchoRequest,
	    EchoReply,
	    DestinationUnreachable,
	    SourceQuench,
	    TimeExpired
	};
	
	enum { SIZE = 32 };

	const SubType
	Type;

	ICMPPacket(SubType type)
	    : IPPacket(0,0,IPPacket::ICMP)
	    , Type(type)
	{}
	~ICMPPacket(){}
    };

    template<ICMPPacket::SubType _T>
    struct ICMPPacketSubType
    {
	ICMPPacketSubType() : ICMPPacketSubType(_T){}
    };

    typedef ICMPPacketSubType<ICMPPacket::EchoRequest>
    ICMPEchoRequest;

    typedef ICMPPacketSubType<ICMPPacket::EchoReply>
    ICMPEchoReply;
}

#endif // NETWORK__ICMPPACKET_HXX__
