#include "Simulation/Timer.h++"
#include "Protocols/Ethernet.h++"
#include "Node.h++"

namespace ARP
{
    void
    Node::
    Flush()
    {
	this->ResolutionTable_.clear();
	this->Pending_.clear();
    }

    void
    Node::
    Flush(IPAddress ip)
    {
	this->ResolutionTable_.erase(ip);
    }

    bool
    Node::
    Known(IPAddress ip) const
    {
	return ResolutionTable_.count(ip) > 0;
    }

    Node::ResolutionArg
    Node::
    Resolve(IPAddress ip) const
    {
	auto rec = ResolutionTable_.find(ip);
	if (rec != ResolutionTable_.end())
	    return rec->second;
	else
	{
	    ResolutionArg ra = { ResolutionArg::UNKNOWN, 
				 Ethernet::Protocol::INVALID_ADDRESS, ip };
	    return ra;
	}
    }

    void
    Node::
    Request(IPAddress ip)
    {
	if (Pending_.count(ip)) return;
	
	Pending_[ip] = 0;
	Timer::Main.Schedule(this->Timeout(), *this, &Node::TimedOut, ip);
	
	Network::PacketListPtr pktList = 
	    Packet::CreateRequest(ip, this->IPv4Address(), this->EthernetAddress());
	
	if (this->EtherNode_)
	    this->EtherNode_->Send(pktList, Ethernet::Protocol::BROADCAST_ADDRESS);
    }

    void
    Node::
    UpdateTable(IPAddress ip, MACAddress mac, ResolutionArg::StatusCode status)
    {
	ResolutionArg& rec = ResolutionTable_[ip];
	rec = { status, mac, ip };
	if (Pending_.count(ip))
	{
	    Pending_.erase(ip);
	    OnResolved_(rec);
	}
    }

    void
    Node::
    TimedOut(IPAddress ip)
    {
	auto prec = Pending_.find(ip);

	if (prec == Pending_.end()) 
	    return;
	
	if (++(prec->second) > this->Timeouts())
	{
	    UpdateTable(ip, Ethernet::Protocol::INVALID_ADDRESS,
			ResolutionArg::TIMEDOUT);
	
	    Timer::Main.Schedule(EXPIRATION_TIME * Time::SECOND, *this,
				 &Node::Expired, ip);
	}
	else
	{
	    Timer::Main.Schedule(this->Timeout(), *this, &Node::TimedOut, ip);
	    
	    Network::PacketListPtr pktList = 
		Packet::CreateRequest(ip, this->IPv4Address(), this->EthernetAddress());
	    
	    if (this->EtherNode_)
		this->EtherNode_->Send(pktList, Ethernet::Protocol::BROADCAST_ADDRESS);
	}
    }

    void
    Node::
    Expired(IPAddress ip)
    {
	auto rec = ResolutionTable_.find(ip);
	if (rec != ResolutionTable_.end())
	{
	    if (rec->second.Status == ResolutionArg::TIMEDOUT)
		ResolutionTable_.erase(rec);
	}
    }
}
