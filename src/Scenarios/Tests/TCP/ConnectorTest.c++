#include <iostream>
#include "SEDNAS.h++"
#include "Simulation/Timer.h++"
#include "Simulation/Event.h++"

#include "Protocols/TCP.h++"
#include "Protocols/IPv4.h++"
#include "Protocols/Ethernet.h++"
#include "Network/PointToPoint.h++"
#include "Network/Packet.h++"

struct NodeStack : public EventHandler<TCP::Connector::ConnectionPtr>
{
    static int G_ID;
    const int ID;
    Ethernet::Node	E;
    IPv4::Node		I;
    TCP::Node		T;
    std::set<TCP::Connector::ConnectionPtr>
    Connections;
    NodeStack()
	: ID(++G_ID)
	, E()
	, I(&E, ID)
	, T(&I)
    {}
    
    operator Ethernet::Node&() { return E; }
    virtual void
    operator()(TCP::Connector::ConnectionPtr c)
    { this->Connections.insert(c); }
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
    TCP::Connector conA(&(a.T), 13);
    TCP::Connector conB(&(b.T), 37);
    conA.OnConnection() += a;
    conB.OnConnection() += b;
    conB.Listening(true);
    conA.Connect(conB.LocalSocket());

    Timer::Main.Run();
    return 0;
}
    
