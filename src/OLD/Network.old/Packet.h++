#ifndef NETWORK__PACKET_HXX__
#define NETWORK__PACKET_HXX__

#include "Protocols/Layers.h++"
#include "Protocols/Protocol.h++"

namespace Network
{
    template<typename _P>
    struct Packet
    {
	const Address<_P> 
	Sender;

	const Address<_P> 
	Receiver;

	const double 
	Size;
	
	const typename Protocol<_P>::ContainedType 
	Type;
	
	virtual ~Packet() {}

    protected:
	Packet(const Address<_P> sender,
	       const Address<_P> receiver,
	       const double size,
	       const typename Protocol<_P>::ContainedType type)
	    : Sender(sender)
	    , Receiver(receiver)
	    , Size(size)
	    , Type(type)
	{}
    };

    template<typename _P, Protocol<_P>::ContainedTypes _T>
    struct DerivedPacket : public Packet<_P>
    {
	typedef typename PacketPayload<_P,_T>::Type PayloadType;

	PayloadType&
	Payload() const
	{ return *(this->Payload_);}
	
	void
	Payload(PayloadType& p)
	{ this->Payload_ = &p; }

	DerivedPacket(const Address<_P>& sender,
		      const Address<_P>& receiver,
		      const double& size,
		      PayloadType* payload)
	    : Packet(sender, receiver, size, _T)
	    , Payload_(payload) {}

	virtual ~Derived(){}
    private:
	PayloadType*
	Payload_;
    };
}
    
#endif // NETWORK__PACKET_HXX__
