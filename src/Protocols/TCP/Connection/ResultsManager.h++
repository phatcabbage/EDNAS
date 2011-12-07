#ifndef TCP__RESULTSMANAGER_HXX__
#define TCP__RESULTSMANAGER_HXX__

#include <cstdio>
#include "../Packet.h++"
#include "../ConnectionManager.h++"

namespace TCP
{
    
    struct ResultsManager : public ConnectionManager
    {
	const int 
	ID;
	
	void
	Created(SeqNumT seq);

	void
	Sent(SeqNumT seq);
	
	void
	Acknowledged(SeqNumT ack);
	
	void
	DuplicateAcknowledged(SeqNumT ack);
	
	void
	TimedOut(SeqNumT seq);

	ResultsManager(Connection& c);
	~ResultsManager();

    protected:
	struct ResultsLogger;

	static int 
	G_ID;
	
	FILE*
	Log;

	void
	RTT(SeqNumT seq, TimeType time);

	void
	Record(SeqNumT seq, const char* event);
    };
}

#endif // TCP__RESULTSMANAGER_HXX__
