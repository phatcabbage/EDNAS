#ifndef IPv4__IPROUTERPORT_HXX__
#define IPV4__IPROUTERPORT_HXX__

#include "IPRouter.h++"
#include "NodeBase.h++"

namespace IPv4
{
    struct IPRouter::Interface : public IPv4::NodeBase
    {
	const InterfaceNumber Number;

	virtual IPv4::Protocol::AddressType
	Address() const
	{ return NodeBase::Address(); }

	virtual void
	Address(Protocol::AddressType ip);

	virtual void
	Received(Network::PacketListPtr& pktList);

	void
	Forward(Network::PacketListPtr& pktList);

	// virtual void
	// Send(Network::PacketListPtr& pkt, Protocol::AddressType receiver,
	//      Protocol::AddressType sender);
	
	virtual void
	Undeliverable(Network::PacketListPtr& pktList)
	{ R.Undeliverable(pktList); }

	virtual void
	Drop(Network::PacketListPtr& pktList);

	Interface(IPRouter& r, 
		  InterfaceNumber number,
		  Protocol::AddressType ip = Protocol::DEFAULT_ADDRESS,
		  Protocol::Netmask     nm = Protocol::CLASS_C,
		  Protocol::AddressType gw = Protocol::DEFAULT_ADDRESS);

	virtual ~Interface(){}
    protected:
	IPRouter& R;

    public:
	std::unique_ptr<Ethernet::Node>
	EtherNode;
    };
}

#endif // IPV4__IPROUTERPORT_HXX__
