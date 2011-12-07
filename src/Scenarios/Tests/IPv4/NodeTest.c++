#include <iostream>

#include "SEDNAS.h++"
#include "Protocols/Ethernet.h++"
#include "Protocols/IPv4.h++"

#include "Network/PointToPoint.h++"

#include "Logging/OStreamLogger.h++"

static Logging::OStreamLogger
NTLog("NodeTest");

IPv4::Protocol::AddressType
G_IP = 0;

struct TalkEthNode : public Ethernet::Node
{
    void Send(Network::PacketListPtr& pktList, 
	      const Ethernet::Protocol::AddressType& receiver)
    {
	Ethernet::Node::Send(pktList, receiver);
	auto& pkt = (*pktList)[Ethernet::Protocol::Layer];
	NTLog << this->Address() << " sending frame:\n\t"
	      
<< pkt->Sender << " -> " << pkt->Receiver << Logging::endl;
    }
    void Received(Network::PacketListPtr& pktList)
    {
	Ethernet::Node::Received(pktList);
	auto& pkt = (*pktList)[Ethernet::Protocol::Layer];
	NTLog << this->Address() << " received frame:\n\t"
	      << pkt->Receiver << " <- " <<pkt->Sender << Logging::endl;
	// NTLog << "\tHandled types: ";
	for (auto it = this->Receivers_.begin(), end = this->Receivers_.end();
	     it != end; ++it)
	    NTLog << it->first << " ";
	NTLog << Logging::endl;
    }
    TalkEthNode() : Ethernet::Node() 
    {
	NTLog << "Creating Ethernet Node:\n\t"
	      << "MAC: " << this->Address() << Logging::endl;
    }
};

struct TalkIPNode : public IPv4::Node
{
    void Send(Network::PacketListPtr& pktList, IPv4::Protocol::AddressType receiver)
    {
	IPv4::Node::Send(pktList, receiver);
	auto& pkt = (*pktList)[Ethernet::Protocol::Layer];
	NTLog << this->Address() << " sending packet:\n\t"
	      << pkt->Sender << " -> " << pkt->Receiver << Logging::endl;
    }
    void Received(Network::PacketListPtr pktList)
    {
	auto& pkt = (*pktList)[Ethernet::Protocol::Layer];
	IPv4::Node::Received(pktList);
	NTLog << this->Address() << " received packet:\n\t"
	      << pkt->Receiver << " <- " << pkt->Sender << Logging::endl;
    }
    void ARPResolved(ARP::Node::ResolutionArg& ra)
    {
	IPv4::Node::ARPResolved(ra);
	NTLog << this->Address() << " resolved MAC address.\n" 
	      << "\tIP:  " << ra.IP << "\n"
	      << "\tMAC: " << ra.MAC << Logging::endl;
    }

    TalkIPNode(Ethernet::Node* s = NULL)
	: IPv4::Node(s, ++G_IP) 
    {
	NTLog << "Creating IPNode:\n\t"
	      << "IP:      " << this->Address() << "\n\t"
	      << "Netmask: " << this->Netmask() << "\n\t"
	      << "Gateway: " << this->Gateway()
	      << Logging::endl;
    }
};

struct NodeStack
{
    std::shared_ptr<TalkEthNode>	E_; 
    std::shared_ptr<TalkIPNode>		I_;
    NodeStack()
	: E_(new TalkEthNode)
	, I_(new TalkIPNode(E_.get()))
    {
	NTLog << "NodeStack created with:\n\t"
	      << E_->Address() << "\n\t"
	      << I_->Address() << Logging::endl;
    }
};

int main(int argc, char** argv)
{
    
    NodeStack a, b;
    auto link = Network::PointToPoint::Create();
    a.E_->Sender(link->LeftHandle());
    b.E_->Sender(link->RightHandle());
    
    Network::PacketListPtr pktList = std::make_shared<Network::PacketList>();
    a.I_->Send(pktList, b.I_->Address());

    Timer::Main.Run();
    return 0;
}
