#ifndef NETWORK__ARP_HXX__
#define NETWORK__ARP_HXX__

#include "./Packet.h++"
#include "./Ethernet.h++"
#include "./IPv4.h++"

namespace Network
{
    struct ARP
    {
	typedef Ethernet ContainerProtocol;
	static const ContainerProtocol::SubType
	TypeCode = Ethernet::ARP;
	enum SubType
	{
	    Reply,
	    Request
	};
	struct AddressType
	{
	    IPv4::AddressType     IP;
	    Ethernet::AddressType MAC;
	    AddressType(IPv4::AddressType ip = 0,
			Ethernet::AddressType mac = 0)
		: IP(ip)
		, MAC(mac)
	    {}
	};
	typedef Packet<ARP> PacketType;
    };

    template<> struct PacketSize<ARP>{ enum { SIZE = 208 }; };

    // template<>
    // Packet<ARP>
    // ::Packet(ARP::SubType type,
    // 	     double size,
    // 	     ARP::AddressType sender,
    // 	     ARP::AddressType receiver)
    // 	: Packet<Ethernet>(Ethernet::ARP, PacketSize<ARP>::SIZE + size)
    // 	, Type(type)
    // 	, Sender(sender)
    // 	, Receiver(receiver)
    // {}

    struct ARPRequest
    {
	typedef ARP ContainerProtocol;
	const static ContainerProtocol::SubType TypeCode = ARP::Request;
	typedef Packet<ARPRequest> PacketType;
    };

    struct ARPReply
    {
	typedef ARP ContainerProtocol;
	const static ContainerProtocol::SubType TypeCode = ARP::Reply;
	typedef Packet<ARPReply> PacketType;
    };
    
    template<>
    struct Packet<ARPRequest> : public Packet<ARP>
    {
	Packet(IPv4::AddressType senderIP,
	       Ethernet::AddressType senderMAC,
	       IPv4::AddressType receiverIP)
	    : Packet<ARP>(ARPRequest::TypeCode,
			  0,
			  ARP::AddressType(senderIP, senderMAC),
			  ARP::AddressType(receiverIP, Ethernet::AddressType(0)))
	{}
    };

    template<>
    struct Packet<ARPReply> : public Packet<ARP>
    {
	Packet(IPv4::AddressType senderIP,
	       Ethernet::AddressType senderMAC,
	       IPv4::AddressType receiverIP,
	       Ethernet::AddressType receiverMAC)
	    : Packet<ARP>(ARPReply::TypeCode,
			  0,
			  ARP::AddressType(senderIP, senderMAC),
			  ARP::AddressType(receiverIP, receiverMAC))
	{}
    };
    typedef Packet<ARP> ARPPacket;
    typedef Packet<ARPRequest> ARPRequestPacket;
    typedef Packet<ARPReply> ARPReplyPacket;
}

#endif // NETWORK__ARP_HXX__
