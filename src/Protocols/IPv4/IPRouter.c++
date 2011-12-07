#include <algorithm>
#include "Logging/OStreamLogger.h++"
#include "IPRouter.h++"
#include "IPRouterPort.h++"

namespace IPv4
{
    static 
    Logging::OStreamLogger
    IPRLog("<IPRouter>");

    IPRouter::
    IPRouter()
	: Interfaces_()
	, RoutingTable_()
    {
	IPRLog<< "Created IPRouter." << Logging::endl;
    }

    IPRouter::
    ~IPRouter()
    {
	while (!Interfaces_.empty())
	{
	    delete Interfaces_.back();
	    Interfaces_.pop_back();
	}
    }   
	    
    void
    IPRouter::
    AddRoute(Protocol::AddressType subnet, InterfaceNumber i)
    {
	RoutingTable_[subnet] = i;
    }

    void 
    IPRouter::
    Received(Network::PacketListPtr& pktList, InterfaceNumber n)
    {
	// IPRLog<< "Received packet!" << Logging::endl;
	this->Forward(pktList, n);
    }
    
    void
    IPRouter::
    Forward(Network::PacketListPtr& pktList, InterfaceNumber from)
    {
	// IPRLog<< "Horrible forward logic!" << Logging::endl;
	int ip = (*pktList)[IPv4::Protocol::Layer]->Receiver;

	int bestSubnet = 0;
	int bestScore = -1;
	for(auto it = RoutingTable_.begin(), end = RoutingTable_.end();
	    it != end; ++it)
	{
	    int score = ip & it->first;
	    if (score > bestScore && it->second != from)
	    {
		bestScore = score;
		bestSubnet = it->first;
	    }
	}

	if (bestScore == -1)
	{
	    // IPRLog << "Undeliverable: R,IP,GW\n"
	    // 	   << IPv4::AddressType(ip) << ","
	    // 	   << Interfaces_[from]->Address() << ","
	    // 	   << Interfaces_[from]->Gateway() << Logging::endl;
	    Undeliverable(pktList);
	}
	else
	{
	    InterfaceNumber iface = RoutingTable_[bestSubnet];
	    Interfaces_[iface]->Forward(pktList);
	}
    }

    void
    IPRouter::
    Drop(Network::PacketListPtr& pktList)
    {
	// IPRLog<< "Dropping packet." << Logging::endl;
    }

    void
    IPRouter::
    InterfaceAddressChanged(InterfaceNumber i)
    {
	auto it = RoutingTable_.begin(), 
	    end = RoutingTable_.end();

	while (it != end)
	    if (it->second == i) RoutingTable_.erase(it++);
	    else
		++it;

	NodeBase* iface = Interfaces_[i];
	Protocol::AddressType ip = iface->Address();
	this->RoutingTable_[ip] = i;
    }

    void
    IPRouter::
    Undeliverable(Network::PacketListPtr& pktList)
    {
	IPRLog<< "Packet declared undeliverable!" << Logging::endl;
    }

    IPRouter::Interface&
    IPRouter::
    operator[](InterfaceNumber i)
    { return *(Interfaces_[i]);}


    IPRouter::InterfaceNumber
    IPRouter::
    AddInterface(Protocol::AddressType ip,
		 Protocol::Netmask nm,
		 Protocol::AddressType gw)
    {
	InterfaceNumber n = Interfaces_.size();
	Interface* iface = new Interface(*this, n, ip, nm, gw);
	Interfaces_.push_back(iface);
	NodeBase* nodebase = iface;
	RoutingTable_[nodebase->Address()] = n;
	return n;
    }

    void
    IPRouter::Interface::
    Address(Protocol::AddressType ip)
    {
	NodeBase::Address(ip);
	R.InterfaceAddressChanged(this->Number);
    }

    void
    IPRouter::Interface::
    Received(Network::PacketListPtr& pktList)
    { 
	R.Received(pktList, Number); 
    }

    IPRouter::Interface::
    Interface(IPRouter& r, 
	      IPRouter::InterfaceNumber number,
	      Protocol::AddressType ip,
	      Protocol::Netmask nm,
	      Protocol::AddressType gw)
	: NodeBase(NULL, ip, nm, gw)
	, Number(number)
	, R(r)
	, EtherNode(new Ethernet::Node)
    {
	this->NodeBase::Sender(EtherNode.get());
    }

    void
    IPRouter::Interface::
    Forward(Network::PacketListPtr& pktList)
    {
	auto& pkt = (*pktList)[IPv4::Protocol::Layer];
	IPv4::Protocol::AddressType sender = pkt->Sender;
	IPv4::Protocol::AddressType receiver = pkt->Receiver;
//	NodeBase::Send(pktList, receiver, sender);
	Send(pktList, receiver, sender);
    }

    // void
    // IPRouter::Interface::
    // Send(Network::PacketListPtr& pkt, Protocol::AddressType receiver,
    // 	 Protocol::AddressType sender)
    // {
    // 	    auto linkHandle = Sender_->Sender();
    // 	    if (linkHandle && linkHandle->QueuedItems() >= MaxBuffer_)
    // 	    {
    // 		IPRLog << "Dropping packet due to insufficient buffer space."
    // 		       << Logging::endl;
    // 		Drop(pkt);
    // 		return;
    // 	    }
    // 	}
    // 	NodeBase::Send(pkt, receiver, sender);
    // }

    void
    IPRouter::Interface::
    Drop(Network::PacketListPtr& pkt)
    {
    }
}
