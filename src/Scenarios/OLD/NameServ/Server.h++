#ifndef SCENARIOS__NAMESERV__SERVER_HXX__
#define SCENARIOS__NAMESERV__SERVER_HXX__

#include "Protocols/DNS.h++"
#include "Network/Node.h++"

#include <map>
#include <string>

namespace PD = Protocol::DNS;

namespace Scenarios
{
    namespace NameServ
    {
	class Server 
	    : public Net::Node
	    , PD::ServerInterface
	{
	private:
	    typedef std::map<std::string, Net::NodeAddress> HostMap;
	    
	    HostMap
	    Hosts_;

	protected:
	    virtual void
	    AnswerRequest(QueryMessage* const msg);

	public:
	    virtual void
	    Received(QueryMessage* const msg);

	    virtual void
	    Received(UpdateMessage* const msg);
	};
    }
}

#endif // SCENARIOS__NAMESERV__SERVER_HXX__
