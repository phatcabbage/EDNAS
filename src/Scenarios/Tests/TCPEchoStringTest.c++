#include "SEDNAS.h++"
#include "Simulation/Timer.h++"

#include "Protocols/Echo.h++"

#include "Nodes/Bridge.h++"
#include "Nodes/DHCPClient.h++"
#include "Nodes/DHCPServer.h++"
#include "Nodes/Pinger.h+"
#include "Nodes/TCPSocket.h++"

struct Node
{
    static int G_ID;
    const  int ID;
    Nodes::EthernetNode E_;
    Nodes::IPNode	I_;
    Nodes::TCPNode	T_;
    Nodes::UDPNode	U_;
    Nodes::Pinger	P_;

    Node()
	: ID(++G_ID)
	, E_()
	, I_(E_)
	, T_(I_)
	, U_(I_)
	, P_(I_)
    {}
    virtual ~Node(){}
};

int Node::G_ID = 0;

struct Server : public Node
{
    Nodes::DHCPServer	D_;
    Nodes::TFTPServer	TF_;
    
    Server()
	: Node()
	, D_(U_)
	, TF_(U_)
    {}
    virtual ~Server(){}
};
