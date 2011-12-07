#ifndef NODES__DHCPSERVER_HXX__
#define NODES__DHCPSERVER_HXX__

#include <map>
#include <set>

#include "Core/Signaling/SingleEvent.h++"
#include "Core/Signaling/EventAdapter.h++"

#include "Logging/Logged.h++"

#include "Protocols/Ethernet.h++"
#include "Protocols/IPv4.h++"
#include "Protocols/DHCP.h++"

#include "Nodes/UDPNode.h++"

namespace Nodes
{
    using Protocols::DHCP;
    
    struct DHCPServer :	public Logging::Logged<DHCPServer>
    {
	IPv4::AddressType DefaultGateway() const
	{ return this->Node_.Node().Gateway(); }

	DHCPServer(UDPNode& node, 
		   IPv4::AddressType start = 1, 
		   IPv4::AddressType end = 254)
	    : Node_(node)
	    , Start_((Node_.Address().Value & IPv4::CLASS_C.Value) + start.Value)
	    , End_((Node_.Address().Value & IPv4::CLASS_C.Value) + end.Value)
	    , Addresses_()
	    , AssignedAddresses_()
	    , ReceivedAdapter_(*this, &DHCPServer::Received)
	{
	    this->Associate();
	    do Addresses_.insert(start++); while (start < end);
	}

	virtual ~DHCPServer()
	{
	    this->Dissociate();
	}

    protected:
	UDPNode& Node_;
	IPv4::AddressType Start_, End_;
	std::set<IPv4::AddressType> Addresses_;
//	std::map<Ethernet::AddressType,IPv4::AddressType>
	std::map<IPv4::AddressType, Ethernet::AddressType>
	AssignedAddresses_;

	EventAdapter<DHCPServer, UDPNode::MessageArg&>
	ReceivedAdapter_;
	
	void
	Associate()
	{ 
	    this->Node_.Bind(this->ReceivedAdapter_, DHCP::ServerPort);
	}

	void
	Dissociate()
	{ 
	    this->Node_.Unbind(DHCP::ServerPort);
	}

	void
	Send(DHCP::PacketPtr pkt);

	void
	Received(UDPNode::MessageArg& pkt);

	void
	ReceiveDiscovery(DHCP::PacketPtr& pkt);
	
	void
	SendOffer(DHCP::PacketType::TransactionID tid);
	
	void
	ReceiveRequest(DHCP::PacketPtr& pkt);
	
	void
	SendAcknowledgment(DHCP::PacketType::TransactionID tid,
			   IPv4::AddressType rqAddress);
	
	void
	ReceiveRelease(DHCP::PacketPtr& pkt);
    };
}

#endif // NODES__DHCPSERVER_HXX__
