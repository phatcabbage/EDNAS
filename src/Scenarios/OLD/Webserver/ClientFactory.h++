#ifndef SCENARIOS__PINGSAPLENTY__CLIENTFACTORY_HXX__
#define SCENARIOS__PINGSAPLENTY__CLIENTFACTORY_HXX__

#include "Core.h++"

namespace Scenarios
{
    namespace Webserver
    {
	struct Client;
	class ClientFactory
	{
	private:
	    enum
	    {
		DEF_UP = 32768,
		DEF_DOWN = 131072
	    };

	    BandwidthType 
	    Upstream_;

	    BandwidthType
	    Downstream_;

	public:
	    struct ClientCreateArgs
	    {
		Client* Client_;
		ClientCreateArgs(Client* const c) : Client_(c) {}
	    };

	    Event<ClientCreateArgs>
	    OnCreate;

	    ClientFactory();
	    ~ClientFactory() {}
	    
	    BandwidthType
	    Upstream() const { return this->Upstream_;}

	    BandwidthType
	    Downstream() const { return this->Downstream_;}

	    void
	    SetUpstream(const BandwidthType& bw) { this->Upstream_ = bw; }

	    void
	    SetDownstream(const BandwidthType& bw) { this->Downstream_ = bw;}

	    Client*
	    Create();
	};
    }
}


#endif // SCENARIOS__PINGSAPLENTY__CLIENTFACTORY_HXX__
