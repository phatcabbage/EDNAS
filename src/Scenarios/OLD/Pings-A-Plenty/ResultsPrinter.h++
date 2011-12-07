#ifndef SCENARIOS__PINGSAPLENTY__RESULTSPRINTER_HXX__
#define SCENARIOS__PINGSAPLENTY__RESULTSPRINTER_HXX__

#include <ostream>
#include "Core/Numerical/DataTypes.h++"

namespace Scenarios
{
    namespace PingsAPlenty
    {
	struct Node;
	class ResultsPrinter
	{
	private:
	    void
	    PerNodeComputations(const Node* const node, TimeType& max, TimeType& min, TimeType& mean, TimeType& median, TimeType& stddev) const;
	public:
	    void 
	    StartTimes(std::ostream& o) const;

	    void 
	    EndTimes(std::ostream& o) const;

	    void
	    PingTimes(std::ostream& o) const;

	    void
	    FullStats(std::ostream& o) const;
	};
    }
}

#endif // SCENARIOS__PINGSAPLENTY__RESULTSPRINTER_HXX__
