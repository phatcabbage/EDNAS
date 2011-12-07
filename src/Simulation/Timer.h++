#ifndef SIMULATION__TIMER_HXX__
#define SIMULATION__TIMER_HXX__

#include <map>
#include <utility>

#include "SEDNAS.h++"
#include "Event.h++"

#include "TimerEntry.h++"

class Timer 
{
protected:
    typedef std::multimap<TimeType,ITimerEntry*> TimerEntryContainer;

    struct StateGuard
    {
	bool& S;
	StateGuard(bool& s) : S(s) { S = true; }
	virtual ~StateGuard() { S = false; }
    };

    TimerEntryContainer
    TimerEntries_;

    TimerEntryContainer
    UnmergedTimerEntries_;

    TimeType
    Time_;

    bool
    IsRunning_;

    bool
    IsInUse_;

    void
    Merge()
    {
	if (this->IsInUse_) return;
	this->TimerEntries_.insert
	    (this->UnmergedTimerEntries_.begin(),
	     this->UnmergedTimerEntries_.end());
	this->UnmergedTimerEntries_.clear();
    }

public:
    static
    Timer
    Main;
	
    const TimeType
    Time() const
    { return this->Time_; }
	    
    Timer()
	: TimerEntries_()
	, UnmergedTimerEntries_()
	, Time_(0)
	, IsRunning_(false)
	, IsInUse_(false)
	{}
	
    ~Timer(){}
	
    bool
    isStopped() const
    { return !this->IsRunning_;}

    bool 
    isRunning() const 
    { return this->IsRunning_; }

    void
    Stop()
    { 
	this->UnmergedTimerEntries_.clear();
	this->TimerEntries_.clear();
	this->Time_ = 0;
    }
	
    void
    Run()
    {
	if (this->isRunning()) return;

	StateGuard G(this->IsRunning_);
	do { this->Merge();} while(this->Step());
    };

    bool
    Step()
    {
	if (this->IsInUse_) return false;
	StateGuard G(this->IsInUse_);
	if (this->TimerEntries_.empty()) return false;

	auto it = this->TimerEntries_.begin();

	this->Time_ = it->first;
	(*(it->second))();
	this->TimerEntries_.erase(it);
	return true;
    }

    template<typename _T>
    void
    Schedule(const TimeType& delay, _T& entry)
    {
	TimeType time = delay + Time_;
	this->UnmergedTimerEntries_
	    .insert(std::make_pair(time, new TimerEntry<_T>(entry)));
    }

    template<typename _Arg>
    void
    Schedule(const TimeType& delay, Event<_Arg>& event, _Arg& arg)
    {
	TimeType time = delay + Time_;
	this->UnmergedTimerEntries_
	    .insert(std::make_pair
		    (time, new EventTimerEntry<_Arg>(event, arg)));
    }	

    template<typename _Class, typename _Arg>
    void
    Schedule(const TimeType& delay, _Class& c, void (_Class::*f)(_Arg), const _Arg& a)
    {
	TimeType time = delay + Time_;
	this->UnmergedTimerEntries_
	    .insert(std::make_pair(time, new MemberTimerEntry<_Class, _Arg>(c,f,a)));
    }
    template<typename _Class, typename _Arg>
    void
    Schedule(const TimeType& delay, _Class& c, void (_Class::*f)(_Arg&), _Arg a)
    {
	TimeType time = delay + Time_;
	this->UnmergedTimerEntries_
	    .insert(std::make_pair(time, new MemberTimerEntry<_Class, _Arg>(c,f,a)));
    }
};


#endif // SIMULATION__TIMER_HXX__
