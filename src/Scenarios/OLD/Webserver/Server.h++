#ifndef SCENARIOS__WEBSERVER__SERVER_HXX__
#define SCENARIOS__WEBSERVER__SERVER_HXX__

#include "Core/Signaling/Event.h++"
#include "Core/Numerical/DataTypes.h++"
#include "Foundation/Network/IMessage.h++"
#include "Foundation/Network/Network.h++"
#include "Foundation/Network/Node.h++"
#include "Foundation/Network/NodeMessageContainers/RRMessageContainer.h++"
#include "Protocols/HTTP.h++"

namespace PH = Protocols::HTTP;

namespace Scenarios
{
    namespace Webserver
    {
	struct Client;

	struct Server
	    : public Foundation::Net::TNode<Foundation::Net::BoundedRRMessageContainer>
	    , public PH::ServerInterface
	{
	    enum { DEF_UPSTREAM = 10485760, DEF_DOWNSTREAM = 10485760};
	    const PH::File* Resource;

	    struct RequestArgs
	    {
		Server* const Server_;
		Client* const Client_;
		const PH::URL URI_;
		RequestArgs(Server* const server, Client* const client, const PH::URL& uri)
		: Server_(server), Client_(client), URI_(uri){}
	    };

	    Event<RequestArgs> OnRequestReceived;
	    Event<RequestArgs> OnRequestFulfilled;

	    Server();

	    virtual ~Server() {}

	    uint
	    MaxConnections() const;
	    
	    void
	    SetMaxConnections(uint maxConnections);

	    void
	    Send404(Client* const ci,
		    const PH::URL& uri);

	    void
	    SendFile(Client* const ci,
		     const PH::URL& uri);

	    void
	    SendNotImplemented(Client* const ci,
			       const PH::URL& uri);

	    virtual void
	    Received(Foundation::Net::IMessage* const msg);

	    virtual void
	    Received(PH::GetMessage* const msg);

	    virtual void
	    Received(PH::PutMessage* const msg);

	    virtual void
	    Received(PH::DeleteMessage* const msg);

	    virtual void
	    Sent(PH::OKMessage* const msg);
	};
    }
}

#endif // SCENARIOS__WEBSERVER__SERVER_HXX__
