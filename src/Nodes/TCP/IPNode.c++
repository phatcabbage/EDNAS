#include "Simulation/Timer.h++"
#include "./IPNode.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Nodes::IPNode>::
    Logger("IPNode");
}

namespace Nodes
{
    const TimeType
    IPNode::SendManager::ARPTimeout::Timeout;

    IPNode::
    IPNode(EthernetNode& node, 
	   IPv4::AddressType ip,
	   IPv4::Netmask netmask,
	   IPv4::AddressType gw)
	: OnAddressChanged()
	, EthernetNode_(node)
	, Address_(ip)
	, Netmask_(netmask)
	, Gateway_(gw)
	, SendManager_(new SendManager(*this))
	, Handlers_()
	, ReceivedIPv4_(*this, &IPNode::ReceivedIPv4)
	, ReceivedARP_(*this, &IPNode::ReceivedARP)
    {
	this->EthernetNode_[Ethernet::IPv4] += this->ReceivedIPv4_;
	this->EthernetNode_[Ethernet::ARP] += this->ReceivedARP_;
    }

    IPNode::
    ~IPNode()
    {
	this->EthernetNode_[Ethernet::IPv4] -= this->ReceivedIPv4_;
	this->EthernetNode_[Ethernet::ARP] -= this->ReceivedARP_;
    }

    void
    IPNode::
    Address(IPv4::AddressType ip)
    { 
	AddressChangedEventArgs args = { this, this->Address_, ip};
	Logger << *this << " changing to ";
	this->Address_ = ip; 
	Logger << *this << Logging::endl;
	this->SendManager_->Flush();
	this->OnAddressChanged(args);
    }

    void
    IPNode::
    Netmask(IPv4::Netmask nm)
    {
	this->Netmask_ = nm;
	this->SendManager_->Flush();
    }
    
    void
    IPNode::
    Gateway(IPv4::AddressType gw)
    {
	this->Gateway_ = gw;
    }

    void
    IPNode::
    ReceivedIPv4(Ethernet::PacketPtr& f)
    {
	auto ippkt = std::static_pointer_cast<IPv4::PacketType>
	    (f->Payload);

	MessageArg msgarg(*this, f, ippkt);

	this->Handlers_[ippkt->Type](msgarg);
    }

    void
    IPNode::
    ReceivedARP(Ethernet::PacketPtr& f)
    {
	auto arppkt = std::static_pointer_cast<ARP::PacketType>
	    (f->Payload);
	
	this->SendManager_->UpdateARP
	    (arppkt->SenderIP,
	     arppkt->SenderMAC);

	if (arppkt->Type == ARP::Request)
	{
	    Logger << *this << "Received ARP Request addressed to "
		   << arppkt->ReceiverIP << Logging::endl;

	    if (arppkt->ReceiverIP == this->Address())
	    {
		Logger << *this << "** Realized that's me! ** :)" << Logging::endl;
		auto arpresponse =
		    std::make_shared<ARP::PacketType>
		    (ARP::Reply, 
		     arppkt->SenderIP, 
		     arppkt->SenderMAC,
		     this->Address(), 
		     this->Interface().Address());
		
		auto ethpkt = std::make_shared<Ethernet::PacketType>
		    (Ethernet::ARP, arpresponse, arpresponse->Size(),
		     arppkt->SenderMAC, this->Interface().Address());
		
		Logger << *this << "Responding to " 
		       << (arppkt->SenderIP) << " (MAC: "
		       << (arppkt->SenderMAC) << ")" << Logging::endl;
		
		Logger << *this << "With " << (this->Address()) 
		       << " (MAC: " << (this->Interface().Address())
		       << ")" << Logging::endl;

		this->Interface().Send(ethpkt);
	    }
	}
    }

    bool
    IPNode::
    IsLocalHost(IPv4::AddressType ip) const
    {
	return (ip == IPv4::LOCALHOST || ip == this->Address_);
    }

    bool
    IPNode::
    IsLocalNet(IPv4::AddressType ip) const
    {
	if((ip & Netmask_) == (this->Address_ & Netmask_)) return true;
	if(ip == IPv4::BROADCAST) return true;
	return false;
    }

    void
    IPNode::
    Send(IPv4::PacketPtr& pkt)
    {
	auto ip = pkt->Receiver;
	if (IsLocalHost(ip))			// Addressed to localhost?
	    new LoopbackMessage(*this, pkt);
	else if (IsLocalNet(ip))		// On our subnet?
	    this->SendManager_->Send(pkt);
	else					// All else failed, do
	{					// we have a gateway?
	    if (this->Gateway_ != IPv4::DEFAULT)
		this->SendManager_->Send(pkt, this->Gateway_);
	    else
		Logger << "Undeliverable packet ([" 
		       << pkt->Sender 
		       << "]->["
		       << pkt->Receiver 
		       << "] dropped." << Logging::endl;
	}
    }

    void
    IPNode::
    SendManager::
    Send(IPv4::PacketPtr& pkt)
    {
	auto ip = pkt->Receiver;
	this->Send(pkt, ip);
    };

    void
    IPNode::SendManager::
    Send(IPv4::PacketPtr& pkt, IPv4::AddressType ip)
    {
	auto arpstatus = ARPStatuses[ip];
	
	switch(arpstatus)
	{
	case Unknown:
	    RequestARP(ip);
	    AwaitingARP.insert(std::make_pair(ip, pkt));
	    break;
	case Requested:
	    AwaitingARP.insert(std::make_pair(ip, pkt));
	    break;
	case Known:
	{
	    auto mac = ARPTable[ip];
	    Node.Interface().Send(Ethernet::IPv4, pkt, mac);
	    break;
	}
	case Invalid:
	    Logger << "Undeliverable packet dropped." << Logging::endl;
	    break;
	}
    }

    void
    IPNode::SendManager::
    UpdateARP(IPv4::AddressType ip, Ethernet::AddressType mac)
    {
	if (mac == Ethernet::BROADCAST)
	    ARPStatuses[ip] = Invalid;
	else
	{
	    ARPStatuses[ip] = Known;
	    ARPTable[ip] = mac;
	}

	auto range = AwaitingARP.equal_range(ip);
	for (auto it = range.first; it != range.second; ++it)
	{
	    auto ippkt = it->second;
	    
	    if (mac)
		Node.Interface().Send(Ethernet::IPv4, ippkt, mac);
// 	    else
// 		::Logger << "Undeliverable packet dropped." << Logger::endl;
	}

	AwaitingARP.erase(range.first, range.second);
    }

    void
    IPNode::SendManager::
    CreateDefaultRoutes()
    {
	ARPTable[IPv4::BROADCAST] = Ethernet::BROADCAST;
	ARPStatuses[IPv4::BROADCAST] = Known;
    }

    void
    IPNode::SendManager::
    RequestARP(IPv4::AddressType ip)
    {
	if (ARPStatuses[ip] != Unknown)
	    return;
	
	auto arppkt = std::make_shared<ARP::PacketType>
	    (ARP::Request,
	     ip,				/* Receiver IP */	
	     0,					/* Receiver MAC */
	     Node.Address(),			/* Sender IP */
	     Node.Interface().Address());	/* Sender MAC */
	
	this->Node.Interface().Send(Ethernet::ARP, arppkt, Ethernet::BROADCAST);
	new ARPTimeout(*this, ip);
    }

    IPNode::SendManager::ARPTimeout::
    ARPTimeout(IPNode::SendManager& mgr, IPv4::AddressType ip)
	: SendMgr_(mgr), IP(ip)
    {
	Logger << mgr.Node << "Creating ARP timeout at [Time " << Simulation::Timer::Main.Time()
	       << "] for [Time " << (Simulation::Timer::Main.Time() + Timeout) << "]" << Logging::endl;
	Simulation::Timer::Main.Schedule(Timeout, *this);
    }

    void
    IPNode::SendManager::ARPTimeout::
    operator()()
    {
	if (SendMgr_.ARPStatuses[IP] != Known)
	{
	    Logger << SendMgr_.Node << "ARP Request timed out." << Logging::endl;
	    SendMgr_.UpdateARP(IP, 0);
	}
	delete this;
    }

    IPNode::LoopbackMessage::
    LoopbackMessage(IPNode& node, IPv4::PacketPtr& pkt)
	: Node(node), 
	  Pkt(std::make_shared<Ethernet::PacketType>
	      (Ethernet::IPv4, pkt, pkt->Size()))
    {
	Simulation::Timer::Main.Schedule(0, *this);
    }
    
    void
    IPNode::LoopbackMessage::
    operator()()
    {
	Node.ReceivedIPv4(Pkt);
	delete this;
    }

    std::ostream&
    operator<<(std::ostream& o, const IPNode& w)
    { return o << "[IP " << (w.Address()) << "] "; }
}
