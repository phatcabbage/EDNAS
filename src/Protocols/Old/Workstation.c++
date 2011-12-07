#include <utility>
#include "./Workstation.h++"
/*
namespace Logging
{
    template<>
    OStreamLogger
    Logged<Nodes::Workstation>::
    Logger("Workstation");
}

namespace Nodes
{
    unsigned int
    Workstation::
    G_IP(0);

    Workstation::
    Workstation()
	: IP(++G_IP)
	, ARPTable_()
	, AwaitingARP_()
	, NIC_()
	, NICAdapter_(*this, &Workstation::Received)
    {
	NIC_.OnReceived = this->NICAdapter_;
	Logger << *this << " Created." << Logging::endl;
    }

    Workstation::
    ~Workstation() 
    {}

    void
    Workstation::
    SendPing(IPv4::AddressType ip)
    {
	Logger << *this << " Set to send ping to |" 
	       << ip << "|" << Logging::endl;
	
	ICMP::PacketPtr icmppkt(new ICMP::PacketType);
	*icmppkt = { ICMP::EchoRequest };
	auto pkts = IPv4::PacketType::Package(ICMP::TypeCode,icmppkt, icmppkt->Size());
	IPv4::PacketPtr pkt = pkts.front();
	this->Send(pkt, ip);
    }		

    void
    Workstation::
    AttemptAwaiting(IPv4::AddressType ip)
    {
	Ethernet::AddressType mac = this->ARPTable_.Resolve(ip);
	if (mac == 0) return;
	if (this->AwaitingARP_.count(ip) <= 0) return;

	IPPacketQueue& list =
	    this->AwaitingARP_[ip];
	
	for(auto it = list.begin(), end = list.end(); it != end; ++it)
	{
	    IPv4::PacketPtr pkt = *it;
	    Ethernet::PacketPtr ethpkt(new Ethernet::PacketType(Ethernet::IPv4, pkt, pkt->Size));
	    ethpkt->Receiver = mac;
	    this->NIC_.Send(ethpkt);
	}
	
	this->AwaitingARP_.erase(ip);
    }

    void
    Workstation::
    Send(IPv4::PacketPtr& pkt, IPv4::AddressType ip)
    {
	Logger << *this << " Sending IPPacket of type " 
	       << pkt->Type << Logging::endl;

	pkt->Sender = this->IP;
	pkt->Receiver = ip;

	Ethernet::AddressType mac 
	    = this->ARPTable_.Resolve(ip);

	if (mac == 0)
	{
	    this->SendARPRequest(ip);
	    this->AwaitingARP_[ip].push_back(pkt);
	}
	else
	{
	    Ethernet::PacketPtr ethpkt(new Ethernet::PacketType(Ethernet::IPv4, pkt, pkt->Size));
	    ethpkt->Receiver = mac;
	    this->NIC_.Send(ethpkt);
	}
    }	

    void
    Workstation::
    Received(Ethernet::PacketPtr& f)
    {
	Logger << *this << " Received Frame of type " 
	       << f->Type << Logging::endl;

	switch(f->Type)
	{
	case Ethernet::IPv4:
	{
	    IPv4::PacketPtr pkt = 
		std::static_pointer_cast<IPv4::PacketType>(f->Payload);
	    if (this->ARPTable_.Update(pkt->Sender, f->Sender))
		this->AttemptAwaiting(pkt->Sender);
	    this->Received(pkt);
	    break;
	}   
	case Ethernet::ARP:
	{
	    ARP::PacketPtr pkt = 
		std::static_pointer_cast<ARP::PacketType>(f->Payload);
	    this->Received(pkt);
	    break;
	}
	default:
	    break;
	}
    }
    
    void
    Workstation::
    Received(IPv4::PacketPtr& pkt)
    {
	Logger << *this << " Received IP Packet of type " 
	       << pkt->Type << Logging::endl;

	if (pkt->Receiver != this->IP) return;

	switch(pkt->Type)
	{
	case IPv4::ICMP:
	{
	    ICMP::PacketPtr icmppkt = 
		std::static_pointer_cast<ICMP::PacketType>(pkt->Payload);
	    this->Received(icmppkt, pkt->Sender);
	    break;
	}
	default:
	    break;
	}
    }

    void
    Workstation::
    Received(ARP::PacketPtr& pkt)
    {
	switch(pkt->Type)
	{
	case ARP::Request:
	{
	    this->HandleARPRequest(pkt);
	    break;
	}
	case ARP::Reply:
	{
	    this->HandleARPReply(pkt);
	    break;
	}
	default:
	    break;
	}
    }
    
    void
    Workstation::
    Received(ICMP::PacketPtr& pkt, IPv4::AddressType senderIP)
    {
	switch(pkt->Type)
	{
	case ICMP::EchoRequest:
	    this->HandleICMPPing(pkt, senderIP);
	    break;
	case ICMP::EchoReply:
	    this->HandleICMPPong(pkt, senderIP);
	    break;
	default:
	    break;
	}
    }

    void
    Workstation::
    SendARPRequest(IPv4::AddressType ip)
    {
	Logger << *this << " Decided to send arp request for "
	       << ip << " -- there are " << this->AwaitingARP_.size()
	       << " pending requests." << Logging::endl;

	if (this->AwaitingARP_.find(ip) != this->AwaitingARP_.end())
	{
	    Logger << *this << " Aborted because there is already "
		   << " a request pending." << Logging::endl;
	    return;
	}
	auto myip = this->IP;
	auto mymac = this->MAC();
	ARP::PacketPtr arppkt(
	    new ARP::PacketType(ARP::Request, myip, mymac, ip, 0));
	Ethernet::PacketPtr ethpkt(
	    new Ethernet::PacketType(Ethernet::ARP, arppkt, arppkt->Size()));
	this->NIC_.Send(ethpkt);
    }

    void
    Workstation::
    HandleARPRequest(ARP::PacketPtr& pkt)
    {
	Logger << *this << " Received ARP Request for "
	       << pkt->ReceiverProtocolAddress << "(IP), "
	       << pkt->ReceiverHardwareAddress << "(MAC)"
	       << " [Our IP is " << this->IP << "]" << Logging::endl;
	if (pkt->ReceiverProtocolAddress != this->IP) return;
	ARP::PacketPtr arppkt(new ARP::PacketType
			      (ARP::Reply, this->IP, this->MAC(),
			       pkt->SenderProtocolAddress, 
			       pkt->SenderHardwareAddress));
	Ethernet::PacketPtr ethpkt(
	    new Ethernet::PacketType(Ethernet::ARP, arppkt, arppkt->Size()));
	this->NIC_.Send(ethpkt);
    }

    void
    Workstation::
    HandleARPReply(ARP::PacketPtr& pkt)
    {
	if (this->ARPTable_.Update(pkt->SenderProtocolAddress,pkt->SenderHardwareAddress))
	    this->AttemptAwaiting(pkt->SenderProtocolAddress);
	Logger << *this << " received an arp reply." << Logging::endl;
    }

    void
    Workstation::
    HandleICMPPing(ICMP::PacketPtr& pkt, IPv4::AddressType senderIP)
    {
	ICMP::PacketPtr pongpkt(new ICMP::PacketType);
	*pongpkt = { ICMP::EchoReply };
	auto ipv4frags = IPv4::PacketType::Package(IPv4::ICMP, pongpkt,
							pongpkt->Size());
	IPv4::PacketPtr ippkt = ipv4frags.front();
	this->Send(ippkt, senderIP);
    }

    void
    Workstation::
    HandleICMPPong(ICMP::PacketPtr& pkt, IPv4::AddressType senderIP)
    {
	Logger << "Pong!" << Logging::endl;
    }

    std::ostream&
    operator<<(std::ostream& o, const Workstation& w)
    { return o << "|" << w.IP << "|"; }
}
*/
