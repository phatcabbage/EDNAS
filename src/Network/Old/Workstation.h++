#ifndef NETWORK__WORKSTATION_HXX__
#define NETWORK__WORKSTATION_HXX__

//#include 

#include <iosfwd>
#include <map>
#include <utility>

#include "Core/Signaling/EventAdapter.h++"
#include "Logging/Logged.h++"

#include "./NIC.h++"
#include "./Ethernet.h++"
#include "./IPv4.h++"
#include "./ARP.h++"
#include "./ICMP.h++"

namespace Network
{
    // typedef unsigned int IPAddress;
    // typedef unsigned int MACAddress;

    // struct ARPPacket;
    // struct Frame;
    // struct ICMPPacket;
    // struct IPPacket;

    struct Workstation;

    struct ARPTable
    {
    private:
	typedef std::map<IPAddress,MACAddress> MapType;
	
	MapType
	Map_;

    public:
	bool 
	Update(IPAddress ip, MACAddress mac)
	{ return this->Map_.insert(std::make_pair(ip,mac)).second; }

	MACAddress
	Resolve(IPAddress ip) const
	{ 
	    MapType::const_iterator it = this->Map_.find(ip);
	    return (it == this->Map_.end()) ? MACAddress(0) : it->second;
	}
	
	ARPTable() : Map_() {}
    };

    
    struct Workstation : public Logging::Logged<Workstation>
    {
	typedef ARPTable
	ARPTableType;

	typedef std::map<IPAddress,std::list<std::shared_ptr<IPPacket> > >
	AwaitingARPType;

	const IPAddress
	IP;

	Workstation();
	~Workstation();

	NIC&
	GetNIC()
	{ return this->NIC_; }

	MACAddress
	MAC() const
	{ return this->NIC_.MAC(); }

	void
	SendPing(IPAddress ip);

    protected:
	static unsigned int 
	G_IP;

	ARPTableType
	ARPTable_;

	AwaitingARPType
	AwaitingARP_;

	NIC
	NIC_;

	EventAdapter<Workstation,Frame&>
	NICAdapter_;

//	void
//	Send(std::shared_ptr<Frame> f)
//	{ this->NIC_.Send(f);}

	// template<typename _P>
	// void
	// Send(std::shared_ptr<Packet<_P>>& pkt

	void
	AttemptAwaiting(IPAddress ip);

	void
	Send(std::shared_ptr<IPPacket> pkt, IPAddress ip);

	void
	Received(Frame& f);
	
	void
	Received(IPPacket* pkt);
	
	void
	Received(ARPPacket* pkt);
	
	void
	Received(ICMPPacket* pkt);

	void
	SendARPRequest(IPAddress ip);
	
	void
	HandleARPRequest(ARPPacket* pkt);

	void
	HandleARPReply(ARPPacket* pkt);
	
	void
	HandleICMPPing(ICMPPacket* pkt);
	
	void
	HandleICMPPong(ICMPPacket* pkt);
    };

    std::ostream&
    operator<<(std::ostream& o, const Workstation& w);
}

#endif // NETWORK__WORKSTATION_HXX__
