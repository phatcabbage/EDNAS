#ifndef NETWORK__IPPACKET_HXX__
#define NETWORK__IPPACKET_HXX__

#include "./Frame.h++"

namespace Network
{
    typedef unsigned int IPAddress;
    struct IPPacket : public Frame
    {
	enum PayloadType
	{
	    ICMP,
	    IGMP,
	    TCP,
	    UDP,
	    OSPF,
	    SCTP
	};

	enum { SIZE = 160 };
	
	IPAddress
	Sender;
	
	IPAddress
	Receiver;
	
	const PayloadType
	Type;
	
	IPPacket(IPAddress sender,
		 IPAddress receiver,
		 PayloadType type
		 double size = 0)
	    : Frame(0,0,size + SIZE,Frame::IPv4)
	    , Sender(sender)
	    , Receiver(receiver)
	    , Type(type)
	{}
	virtual ~IPPacket(){}
    };
}

#endif // NETWORK__IPPACKET_HXX__
