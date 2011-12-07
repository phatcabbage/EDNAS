#include <iostream>
#include "./Node.h++"
#include "Foundation/Simulator/Simulator.h++"
#include "Foundation/Timing/Clock.h++"

namespace PP = Protocols::Ping;

namespace Scenarios
{
    namespace PingsAPlenty
    {
	Node::
	Node(Foundation::Net::Network* net,
	     const BandwidthType& upstream,
	     const BandwidthType& downstream)
	    : Foundation::Net::Node(net, upstream, downstream)
	    , PP::NodeInterface()
	    , LastPing(0)
	    , ReceivedPingCount(0)
	    , PingTimes()
	    , OnPing()
	    , OnPong()
	{
	    std::cout << "[SPPPN] Node created with address " << this->Address() << std::endl;
	}

	void
	Node::
	Sent(PP::PingMessage* const msg)
	{
	    TimeType now = Foundation::Simulator::instance()->Clock()->Time();
	    this->LastPing = now;
	    std::cout << "[SPPPN] (Sent) Ping." << std::endl;
	}

	void
	Node::
	Received(PP::PingMessage* const msg)
	{
	    ++(this->ReceivedPingCount);
	    std::cout << "[SPPPN] (Received) Ping. ";
	    std::cout << this->ReceivedPingCount << " received so far." << std::endl;
	    this->OnPing(*this);
	    std::cout << "[SPPPN] Sending Pong." << std::endl;
	    this->SendPong(dynamic_cast<PP::PongMessage::ReceiverType*>(msg->Sender));

	}
	    
	void
	Node::
	Undeliverable(PP::PingMessage* const msg)
	{
	    std::cout << "[SPPPN] (Undeliverable) Ping." << std::endl;
	}

	void
	Node::
	SendPing(PP::PingMessage::ReceiverType* const that)
	{
	    PP::PingMessage* msg = new PP::PingMessage(this, that);
	    this->Send(msg);
	}
	    
	void
	Node::
	Sent(PP::PongMessage* const msg)
	{
	    std::cout << "[SPPPN] (Sent) Pong." << std::endl;
	}

	void
	Node::
	Received(PP::PongMessage* const msg)
	{
	    OnPong(*this);
	    TimeType now = Foundation::Simulator::instance()->Clock()->Time();
	    this->PingTimes.push_back(TimeEntry(this->LastPing, now));
	    std::cout << "[SPPPN] (Received) Pong. ";
	    std::cout << this->PingTimes.size() << " received so far." << std::endl;
	}

	void
	Node::
	Undeliverable(PP::PongMessage* const msg)
	{
	    std::cout << "[SPPPN] (Undeliverable) Ping." << std::endl;
	}

	void
	Node::
	SendPong(PP::PongMessage::ReceiverType* const that)
	{
	    PP::PongMessage* msg = new PP::PongMessage(this, that);
	    this->Send(msg);
	}
    }
}
