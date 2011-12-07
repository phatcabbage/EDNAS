#include "DHCPServer.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Nodes::DHCPServer>::
    Logger("DHCPServer");
}

namespace Nodes
{
    void
    DHCPServer::
    Send(DHCP::PacketPtr pkt)
    {
	UDP::Port server = DHCP::ServerPort,
	    client = DHCP::ClientPort;
	auto udppkt = std::make_shared<UDP::PacketType>
	    (UDP::DHCP, pkt, pkt->Size(),
	     client, server);
	
	this->Node_.Send(udppkt, IPv4::BROADCAST);
    }
    
    void
    DHCPServer::
    Received(UDPNode::MessageArg& msg)
    {
	
// 	if (pkt->Receiver != BROADCAST)
// 	    return;
	
	auto dhcppkt = 
	    std::static_pointer_cast
	    <DHCP::PacketType>(msg.UDPPkt->Payload);

	Logger << "Received Packet!" << Logging::endl;
	
	switch(dhcppkt->Type)
	{
	case DHCP::Discovery:
	{
	    this->ReceiveDiscovery(dhcppkt);
	    break;
	}
	case DHCP::Request:
	{
	    this->ReceiveRequest(dhcppkt);
	    break;
	}
	case DHCP::Release:
	{
	    this->ReceiveRelease(dhcppkt);
	    break;
	}
	default:
	    break;
	}
    }

    void
    DHCPServer::
    ReceiveDiscovery(DHCP::PacketPtr& pkt)
    {
	Logger << "Received Discovery Request!" << Logging::endl;
	auto tid = pkt->ID;
	
	this->SendOffer(tid);
    }
    
    void
    DHCPServer::
    SendOffer(DHCP::PacketType::TransactionID tid)
    {
	Logger << "Sending offer." << Logging::endl;
	IPv4::AddressType offer =
	    *(this->Addresses_.begin());

	this->Addresses_.erase(this->Addresses_.
			       begin());
	
	auto offerpkt = std::make_shared
	    <DHCP::Message<DHCP::Offer>>
	    (offer, tid, this->DefaultGateway());
	
	this->Send(offerpkt);
    }
    

     void
     DHCPServer::
     ReceiveRequest(DHCP::PacketPtr& pkt)
     {
	 auto tid = pkt->ID;
	 auto address = pkt->Client;
	 
	 Logger << "Request Received!" << Logging::endl;
	 
	 this->SendAcknowledgment(tid, address);
     }

     void
     DHCPServer::
     SendAcknowledgment(DHCP::PacketType::TransactionID tid,
			IPv4::AddressType rqAddress)
     {
	 auto ackpkt = std::make_shared<DHCP::Message<DHCP::
	     Acknowledgment> >(rqAddress, tid);
	 
	 this->Send(ackpkt);
     }

     void
     DHCPServer::
     ReceiveRelease(DHCP::PacketPtr& pkt)
     {
	 auto ip = pkt->Client;
	 AssignedAddresses_.erase(ip);
	 Addresses_.insert(ip);
     }
}
