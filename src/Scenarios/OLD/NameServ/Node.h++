#ifndef SCENARIOS__NAMESERV__NODE_HXX__
#define SCENARIOS__NAMESERV__NODE_HXX__

#include <set>

#include "Util/Events.h++"

#include "Protocols/Ping.h++"
#include "Protocols/DNS.h++"
#include "Protocols/HTTP.h++"

#include "Network/Node.h++"
#include "Network/Network.h++"

namespace PP = Protocols::Ping;
namespace PD = Protocols::DNS;
namespace PH = Protocols::HTTP;

namespace Scenarios
{
    namespace NameServ
    {
	class Client 
	    : Net::Node
	    , public PP::PingInterface
	    , public PD::QuerierInterface
	    , public PH::ClientInterface
	{
	private:
	    PD::ServerInterface* DNSServer_;

	    std::set<PP::PingInterface*>
	    Peers_;
	    
	    std::set<PP::PingInterface*>
	    UnconfirmedPeers_;

	public:
	    
	    Event<PP::PingInterface*>
	    OnAddNewPeer;
	    

	    virtual void
	    Received(PD::AnswerMessage* const msg);
	    
	    virtual void
	    Received(PP::PingMessage* const msg);

	    virtual void
	    Received(PP::PongMessage* const msg);

	    Client(Net::Network* net, PD::ServerInterface* dnsServer = NULL);

	    virtual ~Client() {}
	};
    }
}

#endif // SCENARIOS__NAMESERV__NODE_HXX__
