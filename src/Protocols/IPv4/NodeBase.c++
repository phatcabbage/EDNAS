#include <iostream>
#include "Simulation/Timer.h++"
#include "NodeBase.h++"

namespace IPv4
{
    void
    NodeBase::
    Sender(Ethernet::Node* s)
    {
	if (this->Sender_) (*this->Sender_)[Protocol::ID] -= *this;
	this->Sender_ = s;
	this->ARPResolver_->EthernetNode(s);
	if (this->Sender_) (*this->Sender_)[Protocol::ID] += *this;
    }

    void
    NodeBase::
    Send(Network::PacketListPtr& pktList, Protocol::AddressType receiver,
	 Protocol::AddressType sender)
    {
	Packet::Wrap(pktList, receiver, sender);
	// Protocol::Log << "[IPv4::NodeBase] Attempting to send packet:\n"
	// 	  << "\t" << sender << " -> " << receiver << Logging::endl;
	if (IsLocalHost(receiver))
	    SendLocalHost(pktList);
	else if (IsLocalNet(receiver))
	    SendLocalNet(pktList);
	else if (IsBroadcast(receiver))
	    SendBroadcast(pktList);
	else
	    SendGateway(pktList);
    }
	
    
    void
    NodeBase::
    Send(Network::PacketListPtr& pktList, Ethernet::Protocol::AddressType mac)
    {
	// Protocol::Log << "[NodeBase] Sending..." << Logging::endl;
	if (this->Sender_)
	    this->Sender_->Send(pktList, mac);
	else
	{
	    // Protocol::Log << "[NodeBase] Sender uninititialized." << Logging::endl;
	    Undeliverable(pktList);
	}
    }

    void
    NodeBase::
    ARPResolved(ARP::NodeBase::ResolutionArg& ra)
    {
	// Protocol::Log << "[IPv4::BaseNode] " << this->Address() 
	// 	  << " resolved address for " << ra.IP << "\n"
	// 	  << "\tStatus: ";
	// switch(ra.Status)
	// {
	// case ARP::NodeBase::ResolutionArg::INVALID:
	//     Protocol::Log << "INVALID"; break;
	// case ARP::NodeBase::ResolutionArg::TIMEDOUT:
	//     Protocol::Log << "TIMEDOUT"; break;
	// case ARP::NodeBase::ResolutionArg::VALID:
	//     Protocol::Log << "VALID"; break;
	// case ARP::NodeBase::ResolutionArg::UNKNOWN:
	//     Protocol::Log << "UNKNOWN"; break;
	// };
	// Protocol::Log << Logging::endl;

	auto& list = AwaitingARP_[ra.IP];
	// Protocol::Log << "\tThere are " << list.size() 
	// 	  << " messages awaiting this resolution." << Logging::endl;

	if (ra.Status != ARP::NodeBase::ResolutionArg::VALID)
	{
	    while(!list.empty())
	    {
		Undeliverable(list.front());
		list.pop_front();
	    }
		
	    return;
	}

	while(!list.empty())
	{
	    Send(list.front(), ra.MAC);
	    list.pop_front();
	}

	this->AwaitingARP_.erase(ra.IP);
    }

    void
    NodeBase::
    SendBroadcast(Network::PacketListPtr& pktList)
    {
	// Protocol::Log << *this << " sending packet via broadcast." << Logging::endl;
	this->Send(pktList, Ethernet::Protocol::BROADCAST_ADDRESS);
    }

    void
    NodeBase::
    SendLocalHost(Network::PacketListPtr& pktList)
    {
	// Protocol::Log << *this << " sending packet via localhost." << Logging::endl;

	std::shared_ptr<Network::PacketList> list(pktList);
	Timer::Main.Schedule(1, *this, &NodeBase::SelfReceived, list);
    }

    void
    NodeBase::
    SendLocalNet(Network::PacketListPtr& pktList)
    {
	// Protocol::Log << *this << " sending packet via local network." << Logging::endl;

	auto ip = (*pktList)[Protocol::Layer]->Receiver;

	if (!ARPResolver_->Known(ip))
	{
	    auto& list = this->AwaitingARP_[ip];
	    list.push_back(pktList);
	    // Protocol::Log << "[IPv4::NodeBase] Requesting IP for " << ip << Logging::endl;
	    ARPResolver_->Request(ip);

	    return;
	}

	ARP::NodeBase::ResolutionArg ra = ARPResolver_->Resolve(ip);
	if (ra.Status == ARP::NodeBase::ResolutionArg::VALID)
	    this->Send(pktList, ra.MAC);
	else
	    Undeliverable(pktList);
    }
    
    void
    NodeBase::
    SendGateway(Network::PacketListPtr& pktList)
    {
	// Protocol::Log << *this << " sending packet via gateway (" 
	// 	      << this->Gateway() << ")." << Logging::endl;

	auto ip = this->Gateway_;
	if (ip == Protocol::INVALID_ADDRESS ||
	    ip == Protocol::DEFAULT_ADDRESS ||
	    IsLocalHost(ip) ||
	    !IsLocalNet(ip))
	{
	    Undeliverable(pktList);
	    return;
	}
		
	if (!ARPResolver_->Known(ip))
	{
	    auto& list = this->AwaitingARP_[ip];
	    list.push_back(pktList);
	    ARPResolver_->Request(ip);
	    return;
	}

	ARP::NodeBase::ResolutionArg ra = ARPResolver_->Resolve(ip);
	if (ra.Status == ARP::NodeBase::ResolutionArg::VALID)
	    this->Send(pktList, ra.MAC);
	else
	    Undeliverable(pktList);
    }
    
    void
    NodeBase::
    Undeliverable(Network::PacketListPtr& pktList)
    {
	auto& pkt = (*pktList)[Protocol::Layer];
	auto s = pkt->Sender;
	auto r = pkt->Receiver;
	auto type = pkt->PayloadType;
	// Protocol::Log << "Undeliverable packet.\n"
	// 	      << "\tFrom: " << s << "\n"
	// 	      << "\tTo:   " << r << "\n"
	// 	      << "\tType: " << type << Logging::endl;
    }
}

std::ostream&
operator<<(std::ostream& o, const IPv4::NodeBase& n)
{ return o << "[NodeBase " << n.Address() << "]"; }
