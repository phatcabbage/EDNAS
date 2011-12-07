#ifndef NETWORK__IPNODE_HXX__
#define NETWORK__IPNODE_HXX__

#include <iosfwd>
#include <map>
#include <memory>
#include <utility>

#include "Core/Signaling/EventAdapter.h++"
#include "Core/Signaling/SingleEvent.h++"
#include "Core/Signaling/MultiEvent.h++"
#include "Logging/Logged.h++"

#include "Protocols/Ethernet.h++"
#include "Protocols/IPv4.h++"
#include "Protocols/ARP.h++"
#include "ARPTable.h++"
#include "EthernetNode.h++"

namespace Nodes
{
    using Protocols::Ethernet;
    using Protocols::ARP;
    using Protocols::IPv4;

    struct IPNode : public Logging::Logged<IPNode>
    {
	struct AddressChangedEventArgs;
	struct MessageArg;
	typedef SingleEvent<MessageArg&> MessageEvent;

	MultiEvent<AddressChangedEventArgs&>
	OnAddressChanged;

	EthernetNode&
	Interface()
	{ return this->EthernetNode_; }

	IPv4::AddressType
	Address() const
	{ return this->Address_; }
	
	IPv4::Netmask
	Netmask() const
	{ return this->Netmask_; }
	
	IPv4::AddressType
	Gateway() const
	{ return this->Gateway_; }

	void
	Address(IPv4::AddressType ip);

	void
	Netmask(IPv4::Netmask netmask);

	void
	Gateway(IPv4::AddressType gw);

	void
	Send(IPv4::PacketPtr& pkt);

	MessageEvent&
	operator[](IPv4::SubType type)
	{ return this->Handlers_[type];	}

	IPNode(EthernetNode& node,
	       IPv4::AddressType ip	= IPv4::DEFAULT,
	       IPv4::Netmask netmask	= IPv4::CLASS_C,
	       IPv4::AddressType gw	= IPv4::DEFAULT);
	~IPNode();

    protected:
	struct SendManager;
	
	EthernetNode&
	EthernetNode_;

	IPv4::AddressType
	Address_;

	IPv4::Netmask
	Netmask_;

	IPv4::AddressType
	Gateway_;

	std::shared_ptr<SendManager>
	SendManager_;

	std::map<IPv4::SubType, MessageEvent>
	Handlers_;

	EventAdapter<IPNode,Ethernet::PacketPtr&>
	ReceivedIPv4_;

	EventAdapter<IPNode,Ethernet::PacketPtr&>
	ReceivedARP_;

	void
	ReceivedIPv4(Ethernet::PacketPtr& f);
	
	void
	ReceivedARP(Ethernet::PacketPtr& f);
	
	bool
	IsLocalHost(IPv4::AddressType ip) const;
	
	bool
	IsLocalNet(IPv4::AddressType ip) const;

	struct LoopbackMessage;
    };

    struct IPNode::SendManager
    {
	enum ARPStatus
	{
	    Unknown,
	    Requested,
	    Known,
	    Invalid
	};
	
	void
	Send(IPv4::PacketPtr& pkt);
	
	void
	Send(IPv4::PacketPtr& pkt, IPv4::AddressType ip);
	
	void
	UpdateARP(IPv4::AddressType ip, Ethernet::AddressType mac);

	void
	Flush(){}

	SendManager(IPNode& node)
	    : Node(node)
	    , ARPStatuses()
	    , AwaitingARP()
	    , ARPTable()
	{
	    this->CreateDefaultRoutes();
	}

    protected:
	IPNode& Node;

	std::map<IPv4::AddressType, ARPStatus>
	ARPStatuses;

	std::multimap<IPv4::AddressType, IPv4::PacketPtr>
	AwaitingARP;

	std::map<IPv4::AddressType, Ethernet::AddressType>
	ARPTable;
	
	void
	CreateDefaultRoutes();

	void
	RequestARP(IPv4::AddressType ip);

	struct ARPTimeout;
    };

    struct IPNode::AddressChangedEventArgs
    {
	IPNode*		  Node;
	IPv4::AddressType Old;
	IPv4::AddressType New;
    };

    struct IPNode::MessageArg
    {
	IPNode*		    Node;
	Ethernet::PacketPtr EthPkt;
	IPv4::PacketPtr     IPPkt;
	MessageArg(IPNode&		node, 
		   Ethernet::PacketPtr& ethpkt, 
		   IPv4::PacketPtr&	ippkt)
	    : Node(&node)
	    , EthPkt(ethpkt)
	    , IPPkt(ippkt)
	{}
    };

    struct IPNode::LoopbackMessage
    {
	IPNode& Node;
	Ethernet::PacketPtr Pkt;
	LoopbackMessage(IPNode& node, IPv4::PacketPtr& pkt);
	void operator()();
    };
    
    struct IPNode::SendManager::ARPTimeout
    {
	static const TimeType Timeout = 3 * Time::SECOND;
	IPNode::SendManager& SendMgr_;
	IPv4::AddressType IP;
	ARPTimeout(IPNode::SendManager& mgr, IPv4::AddressType ip);
	void operator()();
    };

    std::ostream&
    operator<<(std::ostream& o, const IPNode& w);
}

#endif // NETWORK__WORKSTATION_HXX__
