#include <iostream>
#include "SEDNAS.h++"
#include "Simulation/Timer.h++"
#include "Logging/OStreamLogger.h++"
#include "Network/Packet.h++"
#include "Network/PointToPoint.h++"
#include "Protocols/TCP.h++"
#include "Protocols/IPv4/IPRouterPort.h++"

struct Greeting : public Network::IData
{
    static const int GREET_PORT = 37;
    bool
    Original() const
    { return Original_; }

    std::string
    Message() const
    { return Message_; }
    
    virtual SizeType
    Size() const
    { return Message_.size() * 1 * Size::MEGABYTE; }

    Greeting(const char* s, bool orig = true) : 
	Original_(orig), Message_(s) {}

    Greeting(std::string s, bool orig = true) :
	Original_(orig), Message_(s) {}

protected:
    bool 
    Original_;

    std::string
    Message_;
};

std::ostream&
operator<<(std::ostream& o, const Greeting& g)
{ 
    o << "[Greeting (" << g.Size() / Size::MEGABYTE << "MB";
    if (g.Original()) o << ",original";
    o << ")] -- \"" << g.Message() << "\"";
    return o;
}

struct NodeStack : public EventHandler<TCP::Connector::ConnectionPtr>
		 , public EventHandler<TCP::Connection::Message&>
{
    Ethernet::Node	E;
    IPv4::Node		I;
    TCP::Node		T;
    TCP::Connector	Greeter;

    std::vector<TCP::Connector::ConnectionPtr>
    Connections;

    std::set<IPv4::Protocol::AddressType>
    Awaiting;

    NodeStack(IPv4::Protocol::AddressType ip,
	      IPv4::Protocol::AddressType gw)
	: E()
	, I(&E, ip, IPv4::Protocol::CLASS_C, gw)
	, T(&I)
	, Greeter(&T, Greeting::GREET_PORT)
	, Connections()
	, Awaiting()
    {
	Greeter.OnConnection() += *this;
	Greeter.Listening(true);
    }

    void
    Greet(IPv4::Protocol::AddressType ip)
    {
	TCP::Socket s = { ip, Greeting::GREET_PORT };
	Awaiting.insert(ip);
	Greeter.Connect(s);
    }
	
    void
    operator()(TCP::Connector::ConnectionPtr conn)
    {
	conn->OnReceive() += *this;
	Connections.push_back(conn);
	bool desired = Awaiting.erase(conn->RemoteIP());
	if (desired)
	    SendGreeting(conn);
    }

    void
    operator()(TCP::Connection::Message& msg)
    {
	auto data = std::static_pointer_cast<Greeting>(msg.Data);
	if (data->Original())
	    ReturnGreeting(msg.Conn, data->Message());
    }

protected:
    void
    SendGreeting(TCP::Connector::ConnectionPtr conn)
    {
	std::shared_ptr<Network::IData> msg = 
	    std::make_shared<Greeting>("Hello!", true);
	conn->Send(msg);
    }

    void
    ReturnGreeting(TCP::Connection& conn, std::string grt)
    {
	std::shared_ptr<Network::IData> msg =
	    std::make_shared<Greeting>(grt + " back to ya!", false);
	conn.Send(msg);
    }
};

struct Subnet
{
    static const int DEFAULT_NODE_COUNT = 10;

    const int ID;

    IPv4::IPRouter 
    Router;

    Ethernet::Bridge
    Bridge;
    
    std::vector<std::shared_ptr<NodeStack> >
    Nodes;

    Subnet(int id, size_t nodes = DEFAULT_NODE_COUNT)
	: ID(id)
	, Router()
	, Bridge()
	, Nodes()
    {
	Nodes.resize(nodes);
	IPv4::Protocol::AddressType subnet = ID << 8;
	IPv4::Protocol::AddressType ip = subnet;
	IPv4::Protocol::AddressType routerip = ++ip;
	Router.AddInterface(routerip);
	Ethernet::NodeBase& routerNode = *(Router[0].Sender());
	Bridge += routerNode;
	for (size_t i = 0; i < nodes; ++i)
	{
	    Nodes[i] = std::make_shared<NodeStack>(++ip, routerip);
	    Bridge += Nodes[i]->E;
	}
    }

    static
    void
    Link(Subnet& a, Subnet& b, IPv4::AddressType subnet)
    {
	subnet <<= 8;
	auto ipA = subnet + 1;
	auto ipB = subnet + 2;
	int ifA = a.Router.AddInterface(ipA, IPv4::Protocol::CLASS_C, ipB);
	int ifB = b.Router.AddInterface(ipB, IPv4::Protocol::CLASS_C, ipA);
	auto link = Network::PointToPoint::Create();
	a.Router[ifA].Sender()->Sender(link->LeftHandle());
	b.Router[ifB].Sender()->Sender(link->RightHandle());
    }
};

Logging::OStreamLogger
RPLog("<PLog>");

struct PacketViewer : EventHandler<Network::PacketListPtr&>,
			   EventHandler<Network::PointToPoint::DropEventArg&>
{
    void
    operator()(Network::PacketListPtr& pktList)
    { Print(pktList); }
    
    void
    operator()(Network::PointToPoint::DropEventArg& dea)
    {
	RPLog << "Packet Dropped. Buffer:\n";
	auto& buffer = dea.Link_->Queue(dea.Side_);
	for(auto it = buffer.begin(), end = buffer.end(); it != end; ++it)
	{
	    auto pktList = *it;
	    size_t size = pktList->size();
	    RPLog << "\t[Packet List] Size: " << size << "\n";
	    for(size_t i = 0; i < size; ++i)
	    {
		auto pkt = pktList->at(i);
		switch(pkt->Type)
		{
		case(Protocols::ETHERNET):
		{
		    auto ethpkt = 
			std::static_pointer_cast<Ethernet::Packet>(pkt);
		    RPLog << "\t\t[Ethernet] S: "
			  << Ethernet::Protocol::AddressType(ethpkt->Sender)
			  << " R: "
			  << Ethernet::Protocol::AddressType(ethpkt->Receiver)
			  << Logging::endl;
		    break;
		}
		case(Protocols::IPv4):
		{
		    auto ippkt = 
			std::static_pointer_cast<IPv4::Packet>(pkt);
		    RPLog << "\t\t[IPv4] S: "
			  << IPv4::Protocol::AddressType(ippkt->Sender)
			  << " R: "
			  << IPv4::Protocol::AddressType(ippkt->Receiver)
			  << Logging::endl;
		    break;
		}
		case(Protocols::TCP):
		{
		    auto tcppkt = 
			std::static_pointer_cast<TCP::Packet>(pkt);
		    RPLog << "\t\t[TCP] S: "
			  << TCP::Protocol::AddressType(tcppkt->Sender)
			  << " R: "
			  << TCP::Protocol::AddressType(tcppkt->Receiver)
			  << "\n";
		    RPLog << "\t\t\tSEQ: " << tcppkt->SEQ
			  << "\tACK: " << tcppkt->ACK
			  << "\tFlags: ";
		    int flags = tcppkt->Flags;
		    if (flags & TCP::Protocol::Synchronize)
			RPLog << "(SYN)";
		    if (flags & TCP::Protocol::Acknowledge)
			RPLog << "(ACK)";
		    if (flags & TCP::Protocol::Data)
			RPLog << "(DATA)";
		    RPLog << Logging::endl;
		    break;
		}
		default:
		    RPLog << "\t\t[(Packet)]" << Logging::endl;
		}
	    }
	}
    }
    
    static
    void
    Time()
    {
	auto time = Timer::Main.Time() ;
	auto sec = time / 1000000000;
	auto msec = (time % 1000000000) / 1000000;
	RPLog << "[Time: " << sec << "." << msec << "]";
    }

    static
    void
    Print(Network::PacketListPtr& pktList)
    {
	if((pktList->at(Ethernet::Protocol::Layer)->Type 
	    != Ethernet::Protocol::ID) ||
	   (pktList->at(IPv4::Protocol::Layer)->Type 
	    != IPv4::Protocol::ID) ||
	   (pktList->at(TCP::Protocol::Layer)->Type 
	    != TCP::Protocol::ID))
	    return;
	
	auto ippkt = (*pktList)[IPv4::Protocol::Layer];
	auto tcppkt = std::static_pointer_cast<TCP::Packet>
	    ((*pktList)[TCP::Protocol::Layer]);
	TCP::Socket s = { ippkt->Sender, tcppkt->Sender };
	TCP::Socket r = { ippkt->Receiver, tcppkt->Receiver };
	Time();
	RPLog << "\nPacket from " << s << " to " << r << "\n";
	RPLog << "\tSEQ: " << tcppkt->SEQ
	      << "\tACK: " << tcppkt->ACK
	      << Logging::endl;
    }
};

struct GreetTimer
{
    typedef std::shared_ptr<NodeStack> NodePtr;
    NodePtr A;
    NodePtr B;
    void operator()()
    { A->Greet(B->I.Address()); delete this; }
    GreetTimer(NodePtr a, NodePtr b, TimeType delay)
	: A(a), B(b)
    { Timer::Main.Schedule(delay, *this); }
};
	

int main(int argc, char** argv)
{
    Subnet A(0), B(2);
    Subnet::Link(A, B, 1);
    auto& nodeA = A.Nodes[2];
    auto& nodeB = B.Nodes[1];
    auto& nodeC = A.Nodes[3];
    new GreetTimer(nodeA, nodeB, 1 * Time::SECOND);
    new GreetTimer(nodeC, nodeB, 3 * Time::SECOND);
    
    PacketViewer rpv;
    nodeA->E.Sender()->OnReceive() += rpv;
    nodeB->E.Sender()->OnReceive() += rpv;
    Network::PointToPoint::OnDrop += rpv;
    Timer::Main.Run();
    return 0;
}
