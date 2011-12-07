#ifndef NETWORK__ARPPACKET_HXX__
#define NETWORK__ARPPACKET_HXX__

#include "./Frame.h++"

namespace Network
{
    typedef unsigned int MACAddress;
    typedef unsigned int IPAddress;

    struct ARPPacket : public Frame
    {
	enum PayloadType
	{
	    Request,
	    Reply
	};

	enum { SIZE = 208 };
	
	MACAddress
	SenderMAC;
	
	IPAddress
	SenderIP;

	MACAddress
	ReceiverMAC;

	IPAddress
	ReceiverIP;
	
	const PayloadType
	Type;
	
	ARPPacket(MACAddress sm,
		  IPAddress  sip,
		  MACAddress rm,
		  IPAddress  rip,
		  PayloadType t)
	    : Frame(0,0,SIZE,Frame::ARP)
	    , SenderMAC(sm)
	    , SenderIP(sip)
	    , ReceiverMAC(rm)
	    , ReceiverIP(rip)
	{}
	~ARPPacket(){}
    };

    struct ARPRequestPacket : public ARPPacket
    {
	ARPRequestPacket(MACAddress sm,
			 IPAddress  sip,
			 IPAddress  rip)
	    : ARPPacket(sm, sip, 0, rip, ARPPacket::Request)
	{}
    };
}

#endif // NETWORK__ARPPACKET_HXX__
