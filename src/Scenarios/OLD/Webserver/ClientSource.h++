#ifndef SCENARIOS__WEBSERVER__CLIENTSOURCE_HXX__
#define SCENARIOS__WEBSERVER__CLIENTSOURCE_HXX__

#include <vector>
#include "Core/Signaling/Event.h++"


namespace Scenarios
{
    namespace Webserver
    {
	struct Client;
	class ClientSource
	{
	private:
	    enum 
	    { 
		DEF_NUM_CLIENTS = 32
	    };

	    std::vector<Client*>
	    Clients_;

	public:
	    ClientSource();
	    virtual ~ClientSource() {}

	    void 
	    Init();
	    
	    const std::vector<Client*>
	    Clients() const { return this->Clients_;}
	};
    }
}

#endif // SCENARIOS__WEBSERVER__CLIENTSOURCE_HXX__
