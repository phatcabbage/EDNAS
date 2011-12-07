#include <iostream>
#include "./NodeSource.h++"
#include "./Scenario.h++"
#include "Core/Numerical/SystemRNG.h++"
#include "Core/Environment/ProgramOptions.h++"
#include "Foundation/Simulator/Simulator.h++"


namespace Scenarios
{
    namespace PingsAPlenty
    {
	NodeSource::
	NodeSource()
	    : Nodes_()
	    , RNG_(new Core::Numerical::SystemRNG<double>())
	{
	}

	void
	NodeSource::
	Init()
	{
	    int numNodes;
	    Core::Environment::ProgramOptions* Options = Core::Environment::ProgramOptions::instance();
	    if (Options->Contains("-nodes"))
		numNodes = Options->Value<int>("-nodes");
	    else
		numNodes = DEF_NUM_NODES;

	    if (numNodes <= 0) return;

	    NodeFactory& factory = Scenario::instance()->Factory;
	    for (int index = 0; index < numNodes; ++index)
	    {
		Node* n = factory.CreateNode();
		this->Nodes_.push_back(n);
		CreateNodeEventArg arg(*this, n);
		this->OnCreate(arg);
	    }
		
	    std::cout << "[SCEN] Node Source initialized." << std::endl;
	}

    }
}
