#ifndef SCENARIOS__WEBSERVER__SCENARIO_HXX__
#define SCENARIOS__WEBSERVER__SCENARIO_HXX__

#include "Foundation/Singleton.h++"
#include "Foundation/Simulator/Simulator.h++"
#include "Protocols/HTTP/File.h++"
#include "./Server.h++"
#include "./ClientSource.h++"
#include "./ClientFactory.h++"
#include "./ClientController.h++"

namespace Scenarios
{
    namespace Webserver
    {
	struct Scenario
	    : public Singleton<Scenario>
	    , public Signaling::Listener<Foundation::Simulator>
	{
	    enum {DEF_FILE_SIZE = 1048576};

	    Protocols::HTTP::File 
	    File;

	    Server 
	    WebServer;
	    
	    ClientSource 
	    Source;
	    
	    ClientFactory 
	    Factory;

	    ClientController
	    Controller;

	    ~Scenario();

	    void Init();
	    void Start();
	    void Stop();
	    void Deinit();
	    
	    void signal(const Signaling::Signal<Foundation::Simulator>& sig);
	protected:
	    Scenario();

	    friend class Singleton<Scenario>;
	};
    }
}
	    

#endif // SCENARIOS__WEBSERVER__SCENARIO_HXX__
