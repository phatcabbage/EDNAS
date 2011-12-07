#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>


#include "./Scenario.h++"
#include "./ResultsPrinter.h++"

namespace Scenarios
{
    namespace PingsAPlenty
    {
	void
	ResultsPrinter::
	PerNodeComputations(const Node* const node,
			    TimeType& max,
			    TimeType& min,
			    TimeType& mean,
			    TimeType& median,
			    TimeType& stddev) const
	{
	    std::vector<TimeType> times;
	    const std::vector<Node::TimeEntry>& nodeTimeEntries = node->PingTimes;
	    const int count = nodeTimeEntries.size();
	    if (!count)
	    {
		max = TimeType(-1);
		min = TimeType(-1);
		mean = TimeType(-1);
		median = TimeType(-1);
		stddev = TimeType(-1);
		return;
	    }

	    times.reserve(count);
	    for(int index = 0; index < count; ++index)
		times.push_back(nodeTimeEntries[index].Received - nodeTimeEntries[index].Sent);
	    
	    std::sort(times.begin(), times.end());
	    max = times.back();
	    min = times.front();

	    if (count % 2)
	    {
		int index = (count / 2) - 1;
		median = (times[index] + times[index + 1]) / 2;
	    }
	    else
	    {
		int index = (count / 2);
		median = times[index];
	    }
	    
	    TimeType sum(0);
	    sum = std::accumulate(times.begin(), times.end(), sum);
	    mean = (sum / (double)count);

	    TimeType varsum(0);
	    for(int index = 0; index < count; ++index)
	    {
		TimeType var(times[index] - mean);
		var *= var.Value();
		varsum += var;
	    }
	    stddev = TimeType(std::sqrt(varsum.Value()));
	}

	void
	ResultsPrinter::
	StartTimes(std::ostream& o) const
	{
	    const std::vector<Node*>& nodes = Scenario::instance()->Source.Nodes();
	    o << std::endl;
	    int nodecount = nodes.size();
	    for(int nodeindex =0; nodeindex < nodecount; ++nodeindex)
	    {
		o << "Node " << nodeindex << "\t";
		Node* node = nodes[nodeindex];
		std::vector<Node::TimeEntry>& times = node->PingTimes;
		int timecount = times.size();
		for(int timeindex = 0; timeindex < timecount; ++timeindex)
		    o << times[timeindex].Sent << "\t";
		o << "\n";
	    }
	    o << std::endl;
	}

	void
	ResultsPrinter::
	EndTimes(std::ostream& o) const
	{
	    const std::vector<Node*>& nodes = Scenario::instance()->Source.Nodes();
	    o << std::endl;
	    int nodecount = nodes.size();
	    for(int nodeindex =0; nodeindex < nodecount; ++nodeindex)
	    {
		o << "Node " << nodeindex << "\t";
		Node* node = nodes[nodeindex];
		std::vector<Node::TimeEntry>& times = node->PingTimes;
		int timecount = times.size();
		for(int timeindex = 0; timeindex < timecount; ++timeindex)
		    o << times[timeindex].Received << "\t";
		o << "\n";
	    }
	    o << std::endl;
	}

	void
	ResultsPrinter::
	PingTimes(std::ostream& o) const
	{
	    const std::vector<Node*>& nodes = Scenario::instance()->Source.Nodes();
	    o << std::endl;
	    int nodecount = nodes.size();
	    for(int nodeindex =0; nodeindex < nodecount; ++nodeindex)
	    {
		o << "Node " << nodeindex << "\t";
		Node* node = nodes[nodeindex];
		std::vector<Node::TimeEntry>& times = node->PingTimes;
		int timecount = times.size();
		for(int timeindex = 0; timeindex < timecount; ++timeindex)
		{ 
		    Node::TimeEntry& te = times[timeindex];
		    o << (te.Received - te.Sent) << "\t";
		}
		o << "\n";
	    }
	    o << std::endl;
	}

	void
	ResultsPrinter::
	FullStats(std::ostream& o) const
	{
	    const std::vector<Node*>& nodes = Scenario::instance()->Source.Nodes();
	    o << std::endl;
	    int count = nodes.size();
	    o << "Node #\tMax\tMin\tMean\tMedian\tStdDev\n";

	    for(int index = 0; index < count; ++index)
	    {
		TimeType max, min, mean, median, stddev;
		const Node* const node = nodes[index];
		this->PerNodeComputations(node, max, min, mean, median, stddev);
		o << index << "\t";
		if (max    >= 0) o << max;    o << "\t";
		if (min    >= 0) o << min;    o << "\t";
		if (mean   >= 0) o << mean;   o << "\t";
		if (median >= 0) o << median; o << "\t";
		if (stddev >= 0) o << stddev; o << "\t";
		o << "\n";
	    }
	    o << std::endl;
	}
    }
}
