#ifndef TCP__DEBUGMANAGER_HXX__
#define TCP__DEBUGMANAGER_HXX__

#include "Logging/OStreamLogger.h++"
#include "../Packet.h++"
#include "../ConnectionManager.h++"

namespace TCP
{
    struct DebugManager : public ConnectionManager
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

	DebugManager(Connection& c);
	virtual ~DebugManager();

    protected:
	typedef std::set<SeqNumT> Records;

	static int 
	G_ID;
	
	Logging::OStreamLogger
	Log;

	Records
	Created_;
	
	Records
	Sent_;
	
	Records
	Acknowledged_;
    };
}

#endif // TCP__DEBUGMANAGER_HXX__
