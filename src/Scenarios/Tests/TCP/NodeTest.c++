#include <iostream>
#include "SEDNAS.h++"
#include "Simulation/Timer.h++"
#include "Simulation/Event.h++"

#include "Protocols/TCP.h++"
#include "Protocols/IPv4.h++"
#include "Protocols/Ethernet.h++"
#include "Network/PointToPoint.h++"
#include "Network/Packet.h++"

struct NodeStack
{
    static int G_ID;
    const int ID;
    Ethernet::Node	E;
    IPv4::Node		I;
    TCP::Node		T;
    NodeStack()
	: ID(++G_ID)
	, E()
	, I(&E, ID)
	, T(&I)
    {}
    
    operator Ethernet::Node&() { return E; }
};

int NodeStack::G_ID = 0;

int main(int argc, char** argv)
{
    Ethernet::Bridge B;
    NodeStack a, b, c, d;
    B += a;
    B += b;
    B += c;
    B += d;

    Network::PacketListPtr pktList =
	std::make_shared<Network::PacketList>();

    (*pktList)[TCP::Protocol::Layer + 1] =
	std::make_shared<Network::IPacket>
	(Protocols::FAKE, Protocols::FAKE, 13, 37, 
	 128 * Bandwidth::KILOBIT, false);
    
    (*pktList)[TCP::Protocol::Layer] =
	std::make_shared<TCP::Packet>();

    c.I.Send(pktList, a.I.Address());

    Timer::Main.Run();
    return 0;
}
    
