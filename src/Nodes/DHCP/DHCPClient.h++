#ifndef NODES__DHCPCLIENT_HXX__
#define NODES__DHCPCLIENT_HXX__

#include "Logging/Logged.h++"

#include "Protocols/IPv4.h++"
#include "Protocols/UDP.h++"
#include "Protocols/DHCP.h++"

#include "UDPNode.h++"

namespace Nodes
{
    using Protocols::IPv4;
    using Protocols::UDP;
    using Protocols::DHCP;

    struct DHCPClient : Logging::Logged<DHCPClient>
    {
	static const TimeType TIMEOUT = 30 * Time::SECOND;
	DHCPClient(UDPNode& node)
	    : Node_(node)
	    , State_(None)
	    , ReceivedAdapter_(*this, &DHCPClient::Received)
	{
	    this->Associate();
	}

	virtual ~DHCPClient()
	{
	    this->Dissociate();
	}

	void
	StartDiscovery();

	void
	operator()()  /* Timer callback -- operation timed out */
	{ this->Timeout(); }

    protected:
	enum State
	{
	    None,
	    Discovery,
	    Requested,
	    Acknowledged
	};

	UDPNode&
	Node_;

	State 
	State_;

	EventAdapter<DHCPClient, UDPNode::MessageArg&>
	ReceivedAdapter_;

	DHCP::PacketType::TransactionID TID;

	void
	Associate()
	{ 
	    this->Node_.Bind(this->ReceivedAdapter_, DHCP::ClientPort);
	}

	void
	Dissociate()
	{
	    this->Node_.Unbind(DHCP::ClientPort);
	}

	void
	Send(DHCP::PacketPtr pkt);

	void
	Received(UDPNode::MessageArg& msg);
	
	void
	ReceivedOffer(DHCP::PacketPtr& pkt);
	
	void
	AcceptOffer(DHCP::PacketPtr& pkt);

	void
	SendRequest(IPv4::AddressType ip);
	
	void
	ReceivedAcknowledgment(DHCP::PacketPtr& pkt);
	
	void
	Release();

	void
	Timeout();
    };
}

#endif // NODES__DHCPCLIENT_HXX__
