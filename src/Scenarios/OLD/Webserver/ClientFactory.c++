#include "Core/Environment/ProgramOptions.h++"
#include "Foundation/Simulator/Simulator.h++"
#include "Foundation/Network/Network.h++"
#include "./Client.h++"
#include "./ClientFactory.h++"

namespace Scenarios
{
    namespace Webserver
    {
	ClientFactory::
	ClientFactory()
	    : Upstream_(DEF_UP)
	    , Downstream_(DEF_DOWN)
	    , OnCreate()
	{
	    Core::Environment::ProgramOptions* Options = Core::Environment::ProgramOptions::instance();
	    if (Options->Contains("-clientup"))
		this->Upstream_ = BandwidthType(Options->Value<double>("-clientup"));
	    if (Options->Contains("-clientdown"))
		this->Downstream_ = BandwidthType(Options->Value<double>("-clientdown"));
	}
	
	Client*
	ClientFactory::
	Create()
	{
	    Foundation::Net::Network* net 
		= Foundation::Simulator::instance()->Network();


	    Client* client =
		new Client(net, this->Upstream_, this->Downstream_);

	    ClientCreateArgs args(client);
	    this->OnCreate(args);
	    
	    return client;
	}
    }
}
