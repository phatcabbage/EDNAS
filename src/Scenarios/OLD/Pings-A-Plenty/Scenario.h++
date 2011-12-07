#ifndef SCENARIOS__PINGSAPLENTY__SCENARIO_HXX__
#define SCENARIOS__PINGSAPLENTY__SCENARIO_HXX__

#include "Foundation/Network/Node.h++"
#include "Foundation/Singleton.h++"
#include "Foundation/Simulator/Simulator.h++"
#include "Protocols/Ping.h++"
#include "SEDNAS.h++"

#include "./NodeSource.h++"
#include "./NodeFactory.h++"

namespace PP = Protocols::Ping;

namespace Scenarios
{
    namespace PingsAPlenty
    {
	struct NodeSource;
	struct NodeFactory;

	struct Scenario :
	    public Singleton<Scenario>,
	    public Signaling::Listener<Foundation::Simulator>
	{
	    Foundation::Simulator*
	    Sim;
	    
	    NodeSource 
	    Source;

	    NodeFactory
	    Factory;

	    void PrintStats();

	    ~Scenario();

	    void Init();
	    void Start();
	    void Stop();
	    void Deinit();

	    void
	    signal(const Signaling::Signal<Foundation::Simulator>& sig);
	protected:
	    Scenario();

	    friend class Singleton<Scenario>;
	};
	    
    }
}


#endif // SCENARIOS__PINGSAPLENTY__SCENARIO_HXX__
