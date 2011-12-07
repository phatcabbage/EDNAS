#ifndef SCENARIOS__PINGSAPLENTY__NODEFACTORY_HXX__
#define SCENARIOS__PINGSAPLENTY__NODEFACTORY_HXX__

#include "./Node.h++"
#include "SEDNAS.h++"

namespace Scenarios
{
    namespace PingsAPlenty
    {
	class NodeFactory
	{
	private:
	    BandwidthType Upstream_;
	    BandwidthType Downstream_;

	public:
	    enum 
	    {
		DEF_UPSTREAM = 14400,
		DEF_DOWNSTREAM = 14400,
	    };

	    BandwidthType GetUpstream() const { return Upstream_; }
	    BandwidthType GetDownstream() const { return Downstream_; }
	    void SetUpstream(const BandwidthType& b) { Upstream_ = b; }
	    void SetDownstream(const BandwidthType& b) { Downstream_ = b; }
	    
	    NodeFactory(const BandwidthType& upstream = BandwidthType(DEF_UPSTREAM),
			const BandwidthType& downstream = BandwidthType(DEF_DOWNSTREAM));
	    
	    ~NodeFactory();

	    Node*
	    CreateNode();
	};
    }//PingsAPlenty
}//Scenarios	

#endif // SCENARIOS__PINGSAPLENTY__NODEFACTORY_HXX__
