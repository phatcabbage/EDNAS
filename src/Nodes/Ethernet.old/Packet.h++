#ifndef ETHERNET__PACKET_HXX__
#define ETHERNET__PACKET_HXX__

#include "Protocol.h++"

namespace Ethernet
{
    struct Packet
    {
	static const SizeType 
	HEADER_SIZE = 304 * Bandwidth::BIT;

	const Ethernet::SubType
	Type;

	SizeType
	Length;

	AddressType
	Receiver;

	AddressType
	Sender;
	
	virtual std::shared_ptr<void>
	Payload() = 0;

	SizeType
	Size() const
	{ return this->Length; }

	Packet(SubType type,
	       SizeType size,
	       AddressType receiver = DEFAULT,
	       AddressType sender = DEFAULT
	    )
	    : Type(type)
	    , Payload(payload)
	    , Length(size + HEADER_SIZE)
	    , Receiver(receiver)
	    , Sender(sender)
	{}

	virtual ~Packet(){}
    };
}

#endif // ETHERNET__PACKET_HXX__
