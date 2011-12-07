#ifndef PROTOCOLS__ECHO_HXX__
#define PROTOCOLS__ECHO_HXX__

#include "SEDNAS.h++"

#include "Network/Packet.h++"

#include "TCP.h++"
#include "UDP.h++"

namespace Protocols
{
    template<typename _P>
    struct Echo
    {
	typedef _P ContainerProtocol;
	static const typename ContainerProtocol::SubType TypeCode = _P::ECHO;
	
	static const typename _P::AddressType ServerPort;// = 7;
	
	struct Packet;
	typedef Packet PacketType;
	typedef std::shared_ptr<Packet> PacketPtr;
    };

    typedef Echo<TCP> TCPEcho;
    typedef Echo<UDP> UDPEcho;

    template<typename _P>
    struct Echo<_P>::Packet
    {
	SizeType 
	Size() const 
	{ return this->Length; }
	
	SizeType Length;

	std::shared_ptr<void> Payload;
	
	Packet(std::shared_ptr<void> payload,
	       SizeType size)
	    : Payload(payload)
	    , Length(size)
	{}
      
	Packet(Network::IPacket* payload)
	    : Payload(payload)
	    , Length(payload->Size())
	{}

	template<typename _T>
	Packet(_T* payloadptr)
	    : Payload(payloadptr)
	    , Length(payloadptr->Size())
	{}
	
	virtual ~Packet(){}
    };
}

#endif // PROTOCOLS__ECHO_HXX__
