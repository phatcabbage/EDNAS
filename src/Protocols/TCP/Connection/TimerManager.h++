#ifndef TCP__TIMERMANAGER_HXX__
#define TCP__TIMERMANAGER_HXX__

#include "SEDNAS.h++"
#include "../ConnectionManager.h++"

namespace TCP
{
    struct TimerManager : public ConnectionManager
    {
	TimerManager(Connection& c)
	    : ConnectionManager(c)
	    , Enabled_(false)
	{}

	void
	TimerExpired(int id);

	bool
	IsSet() const
	{ return Enabled_; }

	void
	Set(TimeType t);

	void
	Disable()
	{ 
	    ++TimerID_; 
	    Enabled_ = false;
	}

	virtual void
	Sent(SeqNumT seq);
	
	virtual void
	Acknowledged(SeqNumT ack);
	
	virtual void
	DuplicateAcknowledged(SeqNumT ack){}
	
	virtual void
	TimedOut(SeqNumT seq){}

    protected:
	int
	TimerID_;

	bool
	Enabled_;
	
	void
	Set();
    };
};

#endif // TCP__TIMERMANAGER_HXX__
