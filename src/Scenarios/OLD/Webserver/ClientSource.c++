#include "Core.h++"
#include "./Client.h++"
#include "./ClientFactory.h++"
#include "./Scenario.h++"
#include "./ClientSource.h++"

namespace Scenarios
{
    namespace Webserver
    {
	ClientSource::
	ClientSource()
	    : Clients_()
	{}
	
	void
	ClientSource::
	Init()
	{
	    Core::Environment::ProgramOptions* Options = Core::Environment::ProgramOptions::instance();
	    int numClients;
	    if (Options->Contains("-clients"))
		numClients = Options->Value<int>("-clients");
	    else
		numClients = DEF_NUM_CLIENTS;

	    numClients -= this->Clients_.size();
	    ClientFactory& factory = Scenario::instance()->Factory;
	    for(int index = 0; index < numClients; ++index)
		this->Clients_.push_back(factory.Create());
	}
    }
}
