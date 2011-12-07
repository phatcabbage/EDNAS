#ifndef TCP__NODE_HXX__
#define TCP__NODE_HXX__

#include <iosfwd>
#include "SEDNAS.h++"
#include "Simulation/Event.h++"
#include "Protocol.h++"
#include "Protocols/IPv4.h++"
#include "Network/Packet.h++"
namespace TCP
{
    struct Server;
    struct Node : public EventHandler<PLP&>
    {
	typedef IPv4::Node SenderType;

	Event<Network::PacketListPtr&>&
	operator[](int port)
	{ return this->Receivers_[port]; }

	SenderType*
	Sender() const
	{ return this->Sender_; }

	IPv4::Protocol::AddressType
	IPAddress() const;

	void
	Sender(SenderType* s);

	Node(SenderType* s = NULL);
	virtual ~Node(){}
	
	virtual void
	Received(PLP& pktList);

	void
	Send(PLP& pktList, IPv4::Protocol::AddressType ip);
	
	virtual void
	operator()(PLP& pktList)
	{ this->Received(pktList); }

	int
	UID() const
	{ return this->UID_; }

    protected:
	const int UID_;
	SenderType* Sender_;

	std::map<int, SingleEvent<Network::PacketListPtr&> >
	Receivers_;

	void
	Drop(Network::PacketListPtr& pktList);
    };
}

std::ostream&
operator<<(std::ostream&,const TCP::Node&);

#endif // TCP__NODE_HXX__
