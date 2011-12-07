#include "./NodeFactory.h++"
#include "Foundation/Simulator/Simulator.h++"

namespace Scenarios
{
    namespace PingsAPlenty
    {
	NodeFactory::
	NodeFactory(const BandwidthType& upstream,
		    const BandwidthType& downstream)
	    : Upstream_(upstream)
	    , Downstream_(downstream)
	{}
	
	NodeFactory::
	~NodeFactory() {}
	
	Node*
	NodeFactory::
	CreateNode()
	{
	    Foundation::Simulator* sim = Foundation::Simulator::instance();
	    return new Node(sim->Network(), this->Upstream_, this->Downstream_);
	}
    }
}
