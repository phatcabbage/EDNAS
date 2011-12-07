#include <iostream>
#include "Simulation/Timer.h++"
#include "Network/PointToPoint.h++"
#include "Protocols/Ethernet/Node.h++"

using Network::PointToPoint;

struct Sink : public EventHandler<Network::PacketListPtr&>
{
    void operator()(Network::PacketListPtr& pktList)
    { std::cout << "Received Packet!" << std::endl; }
    
    Sink(){}
};

int main(int argc, char** argv)
{
    Ethernet::Node a, b;
    Sink s;
    a[Protocols::ProtocolID::FAKE] += s;
    b[Protocols::ProtocolID::FAKE] += s;

    auto link = PointToPoint::Create();
    a.Sender(link->LeftHandle());
    b.Sender(link->RightHandle());
    
    auto pktList = Network::PacketListPtr();
    pktList.reset(new Network::PacketList);
    a.Send(pktList, b.Address());
    
    Timer::Main.Run();
    return 0;
}
