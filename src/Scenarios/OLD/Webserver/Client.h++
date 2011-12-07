#ifndef SCENARIOS__WEBSERVER__CLIENT_HXX__
#define SCENARIOS__WEBSERVER__CLIENT_HXX__

#include <vector>
#include "Core.h++"
#include "Foundation/Network/Node.h++"
#include "Protocols/HTTP.h++"

namespace PH = Protocols::HTTP;

namespace Scenarios
{
    namespace Webserver
    {
	struct Server;
	struct Client 
	    : public Foundation::Net::Node
	    , public PH::ClientInterface
	{
	    TimeType RequestSentTime;
	    std::vector<TimeType> Waits;

	    // Event<Client>
	    // OnRequestSent;
	    
	    // Event<Client>
	    // OnFileReceived;

	    Client(Foundation::Net::Network* net, 
		   const BandwidthType& up,
		   const BandwidthType& down);

	    virtual ~Client(){}
	    
	    void
	    SendGet(Server* si, const PH::URL& uri);

	    void
	    Request(Server* si, const PH::File* const file);
	    
	    void
	    Received(PH::OKMessage* const msg);

	    void
	    Received(PH::NotFoundMessage* const msg);

	    void
	    Received(PH::NotImplementedMessage* const msg);
	    
	    void
	    Sent(PH::GetMessage* const msg);
	}; 
    }
}
#endif // SCENARIOS__WEBSERVER__CLIENT_HXX__
