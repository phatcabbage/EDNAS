#include "Simulation/Timer.h++"
#include "DHCPClient.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Nodes::DHCPClient>::
    Logger("DHCPClient");
}

// namespace Protocols
// {
//     DHCP::PacketType::TransactionID
//     DHCP::Message<DHCP::Discovery>::
//     G_Transaction_ID(0);
// }

namespace Nodes
{
    const TimeType
    DHCPClient::TIMEOUT;

    void
    DHCPClient::
    StartDiscovery()
    {
	auto discpkt = std::make_shared<DHCP::
	    Message<DHCP::Discovery> >();
	this->Send(discpkt);
	this->TID = discpkt->ID;
	this->State_ = Discovery;
	Simulation::Timer::Main.Schedule(TIMEOUT, *this);
    }

    void 
    DHCPClient::
    Send(DHCP::PacketPtr pkt)
    {
	UDP::Port server = DHCP::ServerPort,
	    client = DHCP::ClientPort;
	auto udppkt = std::make_shared<UDP::PacketType>
	    (UDP::DHCP, pkt, pkt->Size(),
	     server, client);
	
	this->Node_.Send(udppkt, IPv4::BROADCAST);
    }

    void
    DHCPClient::
    Received(UDPNode::MessageArg& msg)
    {
	auto dhcppkt = 
	    std::static_pointer_cast
	    <DHCP::PacketType>(msg.UDPPkt->Payload);

	Logger << "Received Packet" << Logging::endl;

	if (this->TID != dhcppkt->ID || this->State_ == None)
	    return;
	
	Logger << "Verified Packet" << Logging::endl;
	
	switch(dhcppkt->Type)
	{
	case DHCP::Offer:
	{
	    this->ReceivedOffer(dhcppkt);
	    break;
	}
	case DHCP::Acknowledgment:
	{
	    this->ReceivedAcknowledgment(dhcppkt);
	    break;
	}
	default:
	{
	    break;
	}
	}
    }

    void
    DHCPClient::
    ReceivedOffer(DHCP::PacketPtr& pkt)
    {
	if (this->State_ == Discovery)
	{
	    this->AcceptOffer(pkt);
	    this->SendRequest(pkt->Client);
	}
    }

    void
    DHCPClient::
    AcceptOffer(DHCP::PacketPtr& pkt)
    {
	auto offerpkt = std::static_pointer_cast<DHCP::OfferMessage>(pkt);

	Logger << "Received offer for [IP "
	       << (offerpkt->Client) << "]" << Logging::endl;
	    
	
	IPNode& node = this->Node_.Node();
	node.Address(offerpkt->Client);
	node.Gateway(offerpkt->DefaultGateway_);
    }

    void
    DHCPClient::
    SendRequest(IPv4::AddressType ip)
    {
	auto reqpkt = 
	    std::make_shared
	    <DHCP::Message<DHCP::Request>>
	    (ip, this->TID);
    
        this->Send(reqpkt);
        this->State_ = Requested;
    }

    void
    DHCPClient::
    ReceivedAcknowledgment(DHCP::PacketPtr& pkt)
    {
	if (!this->State_ == Requested) return;
	this->State_ = Acknowledged;
    }

    void
    DHCPClient::
    Release()
    {
	if (!this->State_ == Acknowledged) return;
	
	auto releasepkt = 
	    std::make_shared
	    <DHCP::Message<DHCP::Release>>
	    (this->Acknowledged);

	this->Send(releasepkt);
        this->State_ = None;
    }

    void
    DHCPClient::
    Timeout()
    {
	this->State_ = None;
    }
}
