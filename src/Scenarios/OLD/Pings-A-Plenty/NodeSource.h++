#ifndef SCENARIOS__PINGSAPLENTY__NODESOURCE_HXX__
#define SCENARIOS__PINGSAPLENTY__NODESOURCE_HXX__

#include <vector>

#include "SEDNAS.h++"
#include "Foundation/Simulator/Simulator.h++"
#include "Core/Numerical/RNG.h++"

#include "./Node.h++"

namespace Scenarios
{
    namespace PingsAPlenty
    {
	class NodeSource
//	    public Signaling::Listener<Foundation::Simulator>
	{
	private:
	    enum {DEF_NUM_NODES = 32};

	    std::vector<Node*>
	    Nodes_;
    
	    Core::Numerical::RNG<double>*
	    RNG_;
    
	public:
	    struct CreateNodeEventArg;
	    Event<CreateNodeEventArg> OnCreate;

	    NodeSource();
	    virtual ~NodeSource() {}

	    void
	    Init();
	    
	    const std::vector<Node*>&
	    Nodes() const { return this->Nodes_;}
	};
	
	struct NodeSource::CreateNodeEventArg
	{
	    NodeSource& Source;
	    Node* const Product;
	    CreateNodeEventArg(NodeSource& source,
			       Node* const product)
		: Source(source), Product(product) {}
	};
    }
}

#endif // SCENARIOS__PINGSAPLENTY__NODESOURCE_HXX__
