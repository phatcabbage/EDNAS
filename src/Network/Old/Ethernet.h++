#ifndef NETWORK__ETHERNET_HXX__
#define NETWORK__ETHERNET_HXX__

#include "./Packet.h++"

namespace Network
{
    struct Ethernet
    {
	typedef void ContainerProtocol;
	enum { BaseSubType = 0};
	enum SubType
	{
	    IPv4,
	    IPv6,
	    ARP,
	    RARP
	};
	typedef Packet<Ethernet> PacketType;
	typedef Address<Ethernet,unsigned int> AddressType;
    };

    template<>
    struct Packet<Ethernet>
    {
	typedef Ethernet::SubType _SubType;
	typedef Ethernet::AddressType _Address;

	const _SubType
	Type;

	double
	Size;

	_Address
	Sender;
	
	_Address
	Receiver;
	
	Packet(_SubType type,
	       double size = 0,
	       _Address sender = _Address(),
	       _Address receiver = _Address())
	    : Type(type)
	    , Size(size + PacketSize<Ethernet>::SIZE)
	    , Sender(sender)
	    , Receiver(receiver)
	{}
	virtual ~Packet(){}
    };

    typedef Ethernet::PacketType Frame;
    typedef Ethernet::AddressType MACAddress;
}


#endif // NETWORK__ETHERNET_HXX__
