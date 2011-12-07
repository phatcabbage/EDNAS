#ifndef SCENARIOS__WEBSERVER__CLIENTCONTROLLER_HXX__
#define SCENARIOS__WEBSERVER__CLIENTCONTROLLER_HXX__

// #include "Core.h++"


namespace Scenarios
{
    namespace Webserver
    {
	struct Client;
	struct ClientController
	{
	    uint OutstandingClients;

	    ClientController();
	    virtual ~ClientController() {}

	    void
	    ClientFileRequestSent(Client* c);
	    
	    void
	    ClientFileReceived(Client* c);

	    void
	    Init();
	};
    }
}

#endif // SCENARIOS__WEBSERVER__CLIENTCONTROLLER_HXX__
