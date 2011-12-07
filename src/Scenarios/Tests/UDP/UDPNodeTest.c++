#include <iostream>
#include "SEDNAS.h++"
#include "Simulation/Timer.h++"
#include "Protocols/Ethernet.h++"
#include "Protocols/IPv4.h++"
#include "Protocols/UDP.h++"

#include "Network/PointToPoint.h++"

struct NodeStack : EventHandler<Network::PacketListPtr&>
{
    static int G_ID;
    Ethernet::Node	E_;
    IPv4::Node		I_;
    UDP::Node		U_;
    
    NodeStack()
	: E_()
	, I_(&E_, ++G_ID)
	, U_(&I_)
    {
	U_[Protocols::FAKE] += *this;
    }

    void operator()(Network::PacketListPtr& pkt)
    {
	std::cout << "[NodeStack] UDP Node received packet!" << std::endl;
    }
};

int NodeStack::G_ID = 0;

int main(int argc, char** argv)
{
    auto link = Network::PointToPoint::Create();
    NodeStack a, b;
    a.E_.Sender(link->LeftHandle());
    b.E_.Sender(link->RightHandle());

    Network::PacketListPtr pktList = std::make_shared<Network::PacketList>();
    a.U_.Send(pktList, b.U_.Address());    
    Timer::Main.Run();
    return 0;
}
