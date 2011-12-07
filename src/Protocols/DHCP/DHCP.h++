#ifndef PROTOCOLS__DHCP_HXX__
#define PROTOCOLS__DHCP_HXX__

#include "SEDNAS.h++"
#include "UDP.h++"

namespace Protocols
{
    using Protocols::IPv4;
    struct DHCP
    {
	typedef UDP ContainerProtocol;

	static const UDP::SubType 
	TypeCode = UDP::DHCP;

	enum SubType
	{
	    Discovery,
	    Offer,
	    Request,
	    Acknowledgment,
	    Release
	};

	struct Packet;
	typedef Packet PacketType;
	typedef std::shared_ptr<Packet> PacketPtr;

	template<SubType S>
	struct Message;

	typedef Message<Discovery>	DiscoveryMessage;
	typedef Message<Offer>		OfferMessage;
	typedef Message<Request>	RequestMessage;
	typedef Message<Acknowledgment>	AcknowledgmentMessage;
	typedef Message<Release>	ReleaseMessage;

	static const UDP::AddressType ServerPort;
	static const UDP::AddressType ClientPort;
    };

    struct DHCP::Packet
    {
	typedef unsigned int TransactionID;

	static const SizeType 
	HEADER_SIZE = 328 * Bandwidth::BIT;

	const DHCP::SubType Type;
	IPv4::AddressType Client;
	const TransactionID ID;

	static SizeType Size() 
	{ return HEADER_SIZE; }

	static const UDP::SubType TypeCode() 
	{ return DHCP::TypeCode; }

	virtual ~Packet(){}

    protected:
	static 
	DHCP::PacketType::TransactionID
	G_Transaction_ID;

	Packet(DHCP::SubType type,
	       IPv4::AddressType client,
	       TransactionID tid
	    )
	    : Type(type)
	    , Client(client)
	    , ID(tid)
	{}
    };

    template<>
    struct DHCP::Message<DHCP::Discovery>
	: public DHCP::Packet
    {
	Message()
	    : Packet(DHCP::Discovery, IPv4::DEFAULT, ++G_Transaction_ID)
	{}
    };

    template<>
    struct DHCP::Message<DHCP::Offer>
	: public DHCP::Packet
    {
	IPv4::AddressType DefaultGateway_;
	Message(IPv4::AddressType address,
		TransactionID tid,
		IPv4::AddressType defaultGateway = IPv4::DEFAULT)
	    : Packet(DHCP::Offer, address, tid)
	    , DefaultGateway_(defaultGateway)
	{}
    };

    template<>
    struct DHCP::Message<DHCP::Request>
	: public DHCP::Packet
    {
	Message(IPv4::AddressType address,
		TransactionID tid)
	    : Packet(DHCP::Request, address, tid)
	{}
    };

    template<>
    struct DHCP::Message<DHCP::Acknowledgment>
	: public DHCP::Packet
    {
	Message(IPv4::AddressType address,
		TransactionID tid)
	    : Packet(DHCP::Offer, address, tid)
	{}
    };
    
    template<>
    struct DHCP::Message<DHCP::Release>
	: public DHCP::Packet
    {
	Message(IPv4::AddressType address)
	    : Packet(DHCP::Release, address, 0)
	{}
    };
};

#endif // PROTOCOLS__UDP_HXX__
