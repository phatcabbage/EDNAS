#ifndef TCP__CONNECTIONMANAGER_HXX__
#define TCP__CONNECTIONMANAGER_HXX__

#include "Protocol.h++"

namespace TCP
{
    struct Connection;
    struct ConnectionManager
    {
	virtual void 
	Created(SeqNumT seq){}

	virtual void
	Sent(SeqNumT seq){}

	virtual void
	Acknowledged(SeqNumT ack) = 0;
	
	virtual void
	DuplicateAcknowledged(SeqNumT ack){}
	
	virtual void
	TimedOut(SeqNumT seq){}
	
	ConnectionManager(Connection& c)
	    : C_(c)
	{}

	virtual ~ConnectionManager(){}

    protected:
	Connection& 
	C_;
    };
}

#endif // TCP__CONNECTIONMANAGER_HXX__
