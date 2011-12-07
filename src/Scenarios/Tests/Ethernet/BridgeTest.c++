#include <iostream>
#include <sstream>
#include <vector>
#include "Simulation/Timer.h++"
#include "Protocols/Ethernet/Node.h++"
#include "Protocols/Ethernet/Bridge.h++"
#include "Network/Packet.h++"
#include "Protocols/ProtocolList.h++"

const int NUM_NODES = 3;
const int PINGS_PER_NODE = 2;
const TimeType INTERVAL = 1 * Time::MILLISECOND;

Logging::OStreamLogger Log("<Scenario>");

struct FakePacket : public Network::IPacket
{
    std::string Msg;
    TimeType Start;
    static const int Layer = Ethernet::Protocol::Layer + 1;
    
    FakePacket(const std::string& s) 
	: Network::IPacket(Protocols::ProtocolID::FAKE, Protocols::ProtocolID::FAKE,
			   0, 0, s.size() * 100 * Size::KILOBYTE, true)
	, Msg(s)
	, Start(Timer::Main.Time())
    {}
    virtual ~FakePacket(){}
};

const int FakePacket::Layer;

struct Faker : public EventHandler<Network::PacketListPtr&>
{
    Ethernet::Protocol::AddressType
    Address() const 
    { 
	return this->Sender_ ?
	    this->Sender_->Address() : Ethernet::Protocol::DEFAULT_ADDRESS; 
    }

    Ethernet::Node*
    Sender() const
    { return this->Sender_.get(); }

    void
    Sender(Ethernet::Node* s)
    { 
	this->Dissociate();
	this->Sender_.reset(s); 
	this->Associate();
    }
    
    void
    Send(const std::string& s, Ethernet::Protocol::AddressType mac)
    {
	auto pktList = std::make_shared<Network::PacketList>();
	(*pktList)[FakePacket::Layer] = std::make_shared<FakePacket>(s);
	if (this->Sender_)
	    this->Sender_->Send(pktList, mac);
    }

    void
    Received(const std::string& s, const TimeType& t)
    {
	double sec = t / Time::SECOND;
	Log << "Received: \"" << s << "\" (" << sec << " sec)" << Logging::endl;
    }

    void
    Received(Network::PacketListPtr& pktList)
    {
	auto pkt = static_cast<FakePacket*>(((*pktList)[FakePacket::Layer]).get());
	if (pkt->Type == Protocols::FAKE)
	{
	    TimeType start = pkt->Start;
	    TimeType t = Timer::Main.Time() - start;

	    auto str = pkt->Msg;
	    this->Received(str,t);
	}
    }
    
    Faker()
	: EventHandler<Network::PacketListPtr&>()
	, Sender_(new Ethernet::Node)
    {
	this->Associate();
    } 

    virtual 
    ~Faker()
    {
	this->Dissociate();
    }

    virtual void
    operator()(Network::PacketListPtr& pktList)
    { Received(pktList); }

protected:

    std::unique_ptr<Ethernet::Node>
    Sender_;

    void
    Associate()
    {
	if (this->Sender_) (*this->Sender_)[Protocols::FAKE] += *this;
    }

    void
    Dissociate()
    {
	if (this->Sender_) (*this->Sender_)[Protocols::FAKE] -= *this;
    }
};

struct SendTimer
{
    Faker* Sender;
    Faker* Receiver;
    std::string Msg;

    virtual void operator()()
    {
	if (this->Sender)
	    Sender->Send(Msg, Receiver->Address());
	delete this;
    }

    SendTimer(TimeType time,
	      Faker* sender,
	      Faker* receiver,
	      const std::string& s) 
	: Sender(sender), Receiver(receiver), Msg(s)
    { Timer::Main.Schedule(time, *this); }
};

int main(int argc, char **argv)
{
//     /* Suppress some of the timer's logging output */
//     Logging::Logged<Simulation::Timer>::Verbosity(Logging::Normal);
    /* Amp up the bridge's output */
    Ethernet::Bridge bridge;
    
    std::vector<std::shared_ptr<Faker> > Nodes;

    for (int node = 0; node < NUM_NODES; ++node)
    {
	auto node = std::make_shared<Faker>();
	Nodes.push_back(node);
	bridge += *(node->Sender());
    }

    std::ostringstream oss;    

    for(int sender = 0; sender < NUM_NODES; ++sender)
    {
	auto s = Nodes[sender].get();
	TimeType time = 0;
	for (int recv = 0; recv < NUM_NODES; ++recv)
	{
	    if (recv == sender) continue;
	    auto r = Nodes[recv].get();
	    for(int count = 0; count < PINGS_PER_NODE; ++count)
	    {
		oss.str("");
		oss << "[" << s->Address() << "]->[" << r->Address() << "](" << count << ")";
		new SendTimer(time += INTERVAL, 
			      Nodes[sender].get(),
			      Nodes[recv].get(),
			      oss.str());
	    }
	}
    }

    Timer::Main.Run();
    return 0;
}
    
    
