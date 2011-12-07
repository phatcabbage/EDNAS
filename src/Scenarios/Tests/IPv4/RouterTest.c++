#include <iostream>

#include "SEDNAS.h++"
#include "Protocols/Ethernet.h++"
#include "Protocols/IPv4.h++"
#include "Protocols/IPv4/IPRouterPort.h++"
#include "Network/PointToPoint.h++"

#include "Logging/OStreamLogger.h++"

std::vector<std::shared_ptr<Network::PointToPoint> >
EthLinks;

static Logging::OStreamLogger
MainLog("<Sim>");

void Link(Ethernet::NodeBase& a, Ethernet::NodeBase& b)
{
    auto link = Network::PointToPoint::Create();
    a.Sender(link->LeftHandle());
    b.Sender(link->RightHandle());
    EthLinks.push_back(link);
}

struct Nop
{
    void operator()()
    {
	MainLog << "[NOP!] Time: " << Timer::Main.Time() << Logging::endl;
    }
};

int main(int argc, char **argv)
{
    IPv4::IPRouter router;
    router.AddInterface(0x01);
    router.AddInterface(0x101);
    router.AddInterface(0x201);
    Ethernet::Node ethA, ethB, ethC;
    IPv4::Node nodeA(&ethA), nodeB(&ethB), nodeC(&ethC);

    Link(ethA, *(router[0].EtherNode.get()));
    Link(ethB, *(router[1].EtherNode.get()));
    Link(ethC, *(router[2].EtherNode.get()));
    auto pktList1 = std::make_shared<Network::PacketList>();
    auto pktList2 = std::make_shared<Network::PacketList>();
    (*pktList1)[IPv4::Protocol::Layer + 1] = 
    	std::make_shared<Network::IPacket>
    	(Protocols::FAKE, Protocols::FAKE);
    (*pktList2)[IPv4::Protocol::Layer + 1] = 
    	std::make_shared<Network::IPacket>
    	(Protocols::FAKE, Protocols::FAKE);

    // (*pktList)[Ethernet::Protocol::Layer + 1] =
    // 	std::make_shared<Network::IPacket>
    // 	(Protocols::FAKE, Protocols::FAKE, 0, 0, 1024);

    nodeA.Address(0x02);
    nodeA.Gateway(0x01);
    nodeB.Address(0x102);
    nodeB.Gateway(0x101);
    nodeC.Address(0x202);
    nodeC.Gateway(0x201);

    MainLog << "[Rt 0]: " << router[0].Address() 
	    << "\t" << router[0].Sender()->Address() << "\n"
	    << "[Rt 1]: " << router[1].Address() 
	    << "\t" << router[1].Sender()->Address() << "\n"
	    << "[Rt 2]: " << router[2].Address() 
	    << "\t" << router[2].Sender()->Address() << Logging::endl;

    MainLog << "Node A: " << nodeA.Address() << "\t" 
	    << nodeA.Sender()->Address() << "\n"
	    << "Node B: " << nodeB.Address() << "\t" 
	    << nodeB.Sender()->Address() << "\n"
	    << "Node C: " << nodeC.Address() << "\t" 
	    << nodeC.Sender()->Address() << Logging::endl;



    nodeA.Send(pktList1, nodeB.Address());
    nodeA.Send(pktList2, nodeC.Address());
//    nodeA.Sender()->Send(pktList, nodeB.Sender()->Address());
    Nop nop;
    Timer::Main.Schedule(10 * Time::SECOND, nop);
    Timer::Main.Run();
    return 0;
}
