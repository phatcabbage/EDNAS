#ifndef SCENARIOS__PINGSAPLENTY__NODECONTROLLER_HXX__
#define SCENARIOS__PINGSAPLENTY__NODECONTROLLER_HXX__

#include "Core/Environment/ProgramOptions.h++"
#include "Foundation/Simulator/Simulator.h++"
#include "Foundation/Network/Network.h++"
#include "./Node.h++"
#include "./NodeSource.h++"

namespace Scenarios
{
    namespace PingsAPlenty
    {
	struct NodeController
	{
	    enum { DEF_PINGS_TO_SEND = 1024};
	    EventAdapter<NodeController, NodeSource::CreateNodeEventArg> CreationAdapter;
	    EventAdapter<NodeController, Node> PongAdapter;
	    int PingsToSend;
	    int OutstandingPings;

	    NodeController()
		: CreationAdapter(this, &Scenarios::PingsAPlenty::NodeController::NodeCreated)
		, PongAdapter(this, &Scenarios::PingsAPlenty::NodeController::PongReceived)
		, PingsToSend(0)
		, OutstandingPings(0)
	    {
		NodeSource& Source = Scenario::instance()->Source;
		Source.OnCreate += this->CreationAdapter;
		
		Core::Environment::ProgramOptions* Options = Core::Environment::ProgramOptions::instance();
		PingsToSend = (Options->Contains("-pings") ? (Options->Value<int>("-pings")) : DEF_PINGS_TO_SEND);

		std::cout << "[NC] Created. " << std::endl;
	    }

	    void NodeCreated(const NodeSource::CreateNodeEventArg& arg)
	    {
		Node* node = arg.Product;
		std::cout << "[NC] Associating with Node " << node->Address() << std::endl;
		node->OnPong += this->PongAdapter;
		if (arg.Product->Address() != 1)
		{
		    ++(this->OutstandingPings);
		    this->PongReceived(*node);
		}
	    }	    
	    void PongReceived(const Node& n)
	    {
		--(this->OutstandingPings);
		if (this->PingsToSend)
		{
		    Foundation::Net::Network* network = Foundation::Simulator::instance()->Network();
		    Node* source = dynamic_cast<Node*>(network->NodeFromAddress(n.Address()));
		    Node* target = dynamic_cast<Node*>(network->NodeFromAddress(1));
		    source->SendPing(target);
		    --(this->PingsToSend);
		    ++(this->OutstandingPings);
		}
		else
		{
		    if (!OutstandingPings)
			Foundation::Simulator::instance()->Stop();
		}
	    }
	};
    }
}

#endif
