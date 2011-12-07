#include <algorithm>
#include <sstream>
#include "DebugManager.h++"
#include "../Connection.h++"
#include "Simulation/Timer.h++"

namespace TCP
{
    static
    std::string
    CreateNameFromID(int id)
    {
	std::ostringstream oss;
	oss << "TCP Debug " << id;
	return oss.str();
    }

    struct SeqNumFinder
    {
	SeqNumT SEQ;
	bool 
	operator()(const PacketPtr& pkt) const
	{ 
	    const SeqNumT seq = pkt->SEQ;
	    return seq == SEQ; 
	}
	SeqNumFinder(SeqNumT seq) : SEQ(seq) {}
	SeqNumFinder(const SeqNumFinder& s) : SEQ(s.SEQ) {}
    };

#ifdef _DEBUG
    static
    bool
    ListContainsSeq(std::list<PacketPtr>& l, SeqNumT seq)
    {
	SeqNumFinder SNF(seq);
	return std::find_if(l.begin(), l.end(), SNF) !=
	    l.end();
    }
#endif // _DEBUG

    int
    DebugManager::G_ID;

    DebugManager::
    DebugManager(Connection& c)
	: ConnectionManager(c)
	, ID(++G_ID)
	, Log(CreateNameFromID(ID))
	, Created_()
	, Sent_()
	, Acknowledged_()
    {}
    
    DebugManager::
    ~DebugManager(){}

#ifndef _DEBUG
    void
    DebugManager::
    Created(SeqNumT seq)
    {}

    void
    DebugManager::
    Sent(SeqNumT seq)
    {}

    void
    DebugManager::
    Acknowledged(SeqNumT seq)
    {}

    void
    DebugManager::
    DuplicateAcknowledged(SeqNumT seq)
    {}

    void
    DebugManager::
    TimedOut(SeqNumT seq)
    {}
#else
    void
    DebugManager::
    Created(SeqNumT seq)
    {
	assert(Created_.count(seq) == 0);
	assert(Sent_.count(seq) == 0);
	assert(Acknowledged_.count(seq) == 0);
	Created_.insert(seq);

	assert(ListContainsSeq(C_.OutBuffer_, seq));
	assert(!ListContainsSeq(C_.InFlight_, seq));
	assert(!ListContainsSeq(C_.Completed_, seq));
    }

    void
    DebugManager::
    Sent(SeqNumT seq)
    {
	assert(Created_.count(seq) == 1);
//	assert(Sent_.count(seq) == 0); 
	assert(Acknowledged_.count(seq) == 0);
	Sent_.insert(seq);
	
	assert(!ListContainsSeq(C_.OutBuffer_, seq));
	assert(ListContainsSeq(C_.InFlight_, seq));
	assert(!ListContainsSeq(C_.Completed_, seq));
	
	assert(C_.LWS_.SEQ == seq + 1);
    }
    
    void
    DebugManager::
    Acknowledged(SeqNumT ack)
    {
	SeqNumT seq = ack - 1;
	assert(Created_.count(seq) == 1);
	assert(Sent_.count(seq) == 1);
	assert(Acknowledged_.empty() || 
	       (*Acknowledged_.rbegin()) < ack);
	assert(Acknowledged_.count(seq) == 0);
	assert(Acknowledged_.upper_bound(seq) == 
	       Acknowledged_.end());

	Acknowledged_.insert
	    (Sent_.begin(), Sent_.upper_bound(seq));
	
	assert(!ListContainsSeq(C_.OutBuffer_, seq));
	assert(!ListContainsSeq(C_.InFlight_, seq));
	assert(ListContainsSeq(C_.Completed_, seq));
	assert(!ListContainsSeq(C_.Completed_, ack));
	
	assert(C_.RWS_.ACK == ack);
//	assert(C_.LWS_.SEQ >= ack); // not if it's a re-send
    }

    void
    DebugManager::
    DuplicateAcknowledged(SeqNumT ack)
    {
	SeqNumT seq = ack - 1;
	assert(Created_.count(seq) == 1);
	assert(Sent_.count(seq) == 1);
	assert(Acknowledged_.count(seq) == 1);
	
	assert(!ListContainsSeq(C_.OutBuffer_, seq));
	assert(!ListContainsSeq(C_.InFlight_, seq));
	assert(ListContainsSeq(C_.Completed_, seq));
	
	assert(C_.RWS_.ACK >= ack);
	assert(C_.LWS_.SEQ >= ack);
    }	
    
    void
    DebugManager::
    TimedOut(SeqNumT seq)
    {
	assert(Created_.count(seq) == 1);
	assert(Sent_.count(seq) == 1);
	assert(Acknowledged_.count(seq) == 0);
	
	assert(!ListContainsSeq(C_.OutBuffer_, seq));
	assert(ListContainsSeq(C_.InFlight_, seq));
	assert(!ListContainsSeq(C_.Completed_, seq));
	
	assert(C_.RWS_.ACK <= seq);
    }	
#endif // _DEBUG
}
