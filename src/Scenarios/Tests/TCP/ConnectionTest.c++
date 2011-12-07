#include <iostream>
#include "SEDNAS.h++"
#include "Simulation/Timer.h++"
#include "Simulation/Event.h++"

#include "Protocols/TCP.h++"
#include "Protocols/IPv4.h++"
#include "Protocols/Ethernet.h++"
#include "Network/PointToPoint.h++"
#include "Network/Packet.h++"


struct TestData : public Network::IData
{
    std::string s_;

    virtual SizeType
    Size() const
    { return s_.size() * Size::BYTE; }

    TestData(const std::string& s) : s_(s){}
    TestData(const char* s) : s_(s) {}
};

std::ostream&
operator<<(std::ostream& o, const TestData& t)
{ return o << t.s_; }

struct NodeStack : public EventHandler<TCP::Connector::ConnectionPtr>
		 , public EventHandler<TCP::Connection::Message&>
{
    static int G_ID;
    const int ID;
    Ethernet::Node	E;
    IPv4::Node		I;
    TCP::Node		T;
    std::set<TCP::Connector::ConnectionPtr>
    Connections;
    bool Greeted_;
    NodeStack()
	: ID(++G_ID)
	, E()
	, I(&E, ID)
	, T(&I)
	, Greeted_(false)
    {}

    operator Ethernet::Node&() { return E; }
    virtual void
    operator()(TCP::Connector::ConnectionPtr c)
    { 
	this->Connections.insert(c); 
	c->OnReceive() += *this;
	std::cout << "[NodeStack " << ID << "] *Sending Greeting!" << std::endl;
	this->Greet(*(c.get()));
    }

    virtual void
    operator()(TCP::Connection::Message& msg)
    { 
	auto data = std::static_pointer_cast<TestData>(msg.Data);
	std::cout << "[NodeStack " << ID << "] received message: \""
		<< *data << "\"" << std::endl;

	if (!Greeted_)
	{
	    ReturnGreeting(msg.Conn);
	    Greeted_ = true;
	}
    }

    void
    Greet(TCP::Connection& conn)
    {
	std::shared_ptr<Network::IData> msgA = std::make_shared<TestData>("Hello!");
	conn.Send(msgA);
    }

    void
    ReturnGreeting(TCP::Connection& conn)
    {
	
	std::shared_ptr<Network::IData> msgB = std::make_shared<TestData>("World!");
	conn.Send(msgB);
    }
};

int NodeStack::G_ID = 0;

struct Nop
{
    void operator()()
    { std::cout << "[NOP] Time: " << Timer::Main.Time() << std::endl;}
};

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

    Nop nop;
    Timer::Main.Schedule(10 * Time::SECOND, nop);
    Timer::Main.Run();
    return 0;
}
    
