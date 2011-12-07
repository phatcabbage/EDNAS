#include "Simulation/Timer.h++"
#include "TimerManager.h++"
#include "../Connection.h++"
#include "RTTManager.h++"

namespace TCP
{
    void
    TimerManager::
    TimerExpired(int id)
    { 
	if(id != TimerID_ || Enabled_ == false) return;
	
	Disable();
	C_.TimedOut();
    }

    void
    TimerManager::
    Set(TimeType t)
    {
	assert(t > 0);
	Disable();
	Timer::Main.Schedule(t, *this, 
			     &TimerManager::TimerExpired,
			     TimerID_);
	Enabled_ = true;
    }
    
    void
    TimerManager::
    Set()
    {
	TimeType appRtt = C_.RTTMgr_->Timeout();
	this->Set(appRtt);
    }	

    void
    TimerManager::
    Sent(SeqNumT seq)
    {
	if(!Enabled_)
	    Set();
    }

    void
    TimerManager::
    Acknowledged(SeqNumT ack)
    {
	if(C_.InFlight_.empty())
	    Disable();
	else
	    Set();
    }

}
