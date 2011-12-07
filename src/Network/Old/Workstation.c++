#include <utility>
//#include "./Frame.h++"
#include "./NIC.h++"
#include "./Workstation.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Network::Workstation>::
    Logger("Workstation");
}

namespace Network
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
    SendPing(IPAddress ip)
    {
	Logger << *this << " Set to send ping to |" 
	       << ip << "|" << Logging::endl;
	
	std::shared_ptr<IPPacket> pkt(
	    new ICMPEchoRequestPacket());

	this->Send(pkt, ip);
    }		

    // void
    // Workstation::
    // UpdateArpTable(IPAddress ip, MACAddress mac)
    // {
    // 	std::pair<ARPTableType::iterator,bool> result =
    // 	    ARPTable_.insert(std::make_pair(ip,mac));
	
    // 	bool& replaced = result.second;
    // 	if (replaced)
    // 	{
    // 	    Logger << *this << " updated its ARP cache!" << Logging::endl;
    // 	    if (AwaitingARP_.count(ip) < 1) return;

    // 	    std::pair<IPPacketByIPAddressMapType::iterator,
    // 		IPPacketByIPAddressMapType::iterator> range =
    // 		AwaitingARP_.equal_range(ip);
	    
    // 	    IPPacketByIPAddressMapType::iterator it;
    // 	    IPPacketByIPAddressMapType::iterator& end = range.second;

    // 	    MACAddress thisMac = this->MAC();
    // 	    MACAddress thatMac = mac;

    // 	    for (it = range.first; it != end; ++it)
    // 	    {
    // 		IPPacket* pkt = it->second;
    // 		Frame f(thisMac, thatMac, pkt->Size,
    // 			Frame::IPv4, pkt);
    // 		this->Send(&f);
    // 	    }
    // 	    it = range.first;
    // 	    while(it != end) this->AwaitingARP_.erase(it++);
    // 	}
    // }

    // void
    // Workstation::
    // Send(Frame* f)
    // {
    // 	Logger << *this << " Sending frame of type " 
    // 	       << f->Type << Logging::endl;
    // 	this->NIC_->Send(f);
    // }

    void
    Workstation::
    AttemptAwaiting(IPAddress ip)
    {
	MACAddress mac = this->ARPTable_.Resolve(ip);
	if (mac.Value == 0) return;
	if (!this->AwaitingARP_.count(ip)) return;

	std::list<std::shared_ptr<IPPacket> >& list =
	    this->AwaitingARP_[ip];
	
	for(std::list<std::shared_ptr<IPPacket> >::iterator 
		it = list.begin(), end = list.end();
	    it != end; ++it)
	{
	    std::shared_ptr<IPPacket>& pkt = *it;
	    this->NIC_.Send(pkt, mac);
	}
	
	this->AwaitingARP_.erase(ip);
    }

    void
    Workstation::
    Send(std::shared_ptr<IPPacket> pkt, IPAddress ip)
    {
	Logger << *this << " Sending IPPacket of type " 
	       << pkt->Type << Logging::endl;

	pkt->Sender = this->IP;
	pkt->Receiver = ip;

	MACAddress mac = this->ARPTable_.Resolve(ip);

	if (mac == 0)
	{
	    this->SendARPRequest(ip);
	    this->AwaitingARP_[ip].push_back(pkt);
	}
	else
	{
	    this->NIC_.Send(pkt, mac);
	}
    }	
	
    void
    Workstation::
    Received(Frame& f)
    {
	Logger << *this << " Received Frame of type " << f.Type << Logging::endl;
	switch(f.Type)
	{
	case Ethernet::IPv4:
	    this->Received(static_cast<IPPacket*>(&f));
	    break;
	case Ethernet::ARP:
	    this->Received(static_cast<ARPPacket*>(&f));
	    break;
	default:
	    break;
	}
    }
    
    void
    Workstation::
    Received(IPPacket* pkt)
    {
	Logger << *this << " Received IP Packet of type " << pkt->Type << Logging::endl;

	if (pkt->Receiver != this->IP) return;
	if (this->ARPTable_.Update(pkt->Sender,
				   ((Ethernet::PacketType*)pkt)->Sender))
	    this->AttemptAwaiting(pkt->Sender);

	switch(pkt->Type)
	{
	case IPv4::ICMP:
	    this->Received(static_cast<ICMPPacket*>(pkt));
	    break;
	default:
	    break;
	};
    }

    void
    Workstation::
    Received(ARPPacket* pkt)
    {
	if (pkt->Receiver.MAC != 0 && 
	    this->ARPTable_.Update(pkt->Receiver.IP, pkt->Receiver.MAC))
	    this->AttemptAwaiting(pkt->Receiver.IP);
	if (this->ARPTable_.Update(pkt->Sender.IP, pkt->Sender.MAC))
	    this->AttemptAwaiting(pkt->Sender.IP);
		
	switch(pkt->Type)
	{
	case ARPRequest::TypeCode:
	    this->HandleARPRequest(pkt);
	    break;
	case ARPReply::TypeCode:
	    this->HandleARPReply(pkt);
	    break;
	}
    }
    
    void
    Workstation::
    Received(ICMPPacket *pkt)
    {
	switch(pkt->Type)
	{
	case ICMP::EchoRequest:
	    this->HandleICMPPing(pkt);
	    break;
	case ICMP::EchoReply:
	    this->HandleICMPPong(pkt);
	    break;
	default:
	    break;
	}
    }

    void
    Workstation::
    SendARPRequest(IPAddress ip)
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

	std::shared_ptr<ARPRequestPacket> arprqpkt(
	    new ARPRequestPacket(this->IP, this->MAC(), ip));

	this->NIC_.Send(arprqpkt, this->NIC_.EndPoint()->MAC());
    }

    void
    Workstation::
    HandleARPRequest(ARPPacket* pkt)
    {
	ARP::AddressType& snd = pkt->Sender;
	std::shared_ptr<ARPPacket> arprply(
	    new ARPReplyPacket(this->IP, this->MAC(),
			 snd.IP, snd.MAC));
	this->NIC_.Send(arprply, snd.MAC);
    }

    void
    Workstation::
    HandleARPReply(ARPPacket* pkt)
    {
	Logger << *this << " received an arp reply." << Logging::endl;
//	this->UpdateArpTable(pkt->SenderIP, pkt->SenderMAC);
    }

    void
    Workstation::
    HandleICMPPing(ICMPPacket* pkt)
    {
	std::shared_ptr<ICMPPacket> pongpkt(
	    new ICMPEchoReply::PacketType());
	this->Send(pongpkt, static_cast<IPPacket*>(pkt)->Sender);
    }

    void
    Workstation::
    HandleICMPPong(ICMPPacket* pkt)
    {
	Logger << "Pong!" << Logging::endl;
    }

    std::ostream&
    operator<<(std::ostream& o, const Workstation& w)
    { return o << "|" << w.IP << "|"; }
}
