#ifndef SCENARIOS__PINGSAPLENTY__NODE_HXX__
#define SCENARIOS__PINGSAPLENTY__NODE_HXX__

#include <vector>

#include "Foundation/Network/Node.h++"
#include "Protocols/Ping.h++"
#include "SEDNAS.h++"

namespace PP = Protocols::Ping;

namespace Scenarios
{
    namespace PingsAPlenty
    {
	struct Node : public Foundation::Net::Node
		    , public PP::NodeInterface
	{
	    struct TimeEntry
	    {
		const TimeType Sent;
		const TimeType Received;
		TimeEntry(const TimeType& s,
			  const TimeType& r)
		    : Sent(s), Received(r) {}
		TimeEntry(const TimeEntry& t)
		    : Sent(t.Sent), Received(t.Received) {}

		TimeEntry
		operator=(const TimeEntry& t)
		{ return TimeEntry(t);}
	    };

	    TimeType LastPing;
	    int ReceivedPingCount;

	    std::vector<TimeEntry>
	    PingTimes;

	    Event<Node> OnPing,
		OnPong;

	    Node(Foundation::Net::Network* net,
		 const BandwidthType& upstream = BandwidthType(1048576),
		 const BandwidthType& downstream = BandwidthType(1048576));
		 
	    void
	    Sent(PP::PingMessage* const msg);

	    void
	    Received(PP::PingMessage* const msg);
	    
	    void
	    Undeliverable(PP::PingMessage* const msg);

	    void
	    SendPing(PP::PingMessage::ReceiverType* const that);
	    
	    void
	    Sent(PP::PongMessage* const msg);

	    void
	    Received(PP::PongMessage* const msg);

	    void
	    Undeliverable(PP::PongMessage* const msg);

	    void
	    SendPong(PP::PongMessage::ReceiverType* const that);
	};
    } // PingsAPlenty
}// Scenarios

#endif // SCENARIOS__PINGSAPLENTY__NODE_HXX__
