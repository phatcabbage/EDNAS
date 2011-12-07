#ifndef SIMULATION__TIMERENTRY_HXX__
#define SIMULATION__TIMERENTRY_HXX__

#include "SEDNAS.h++"

struct ITimerEntry
{
    virtual void operator()() = 0;
    virtual ~ITimerEntry(){}
};
    
template<typename _T>
struct TimerEntry : public ITimerEntry
{
    virtual void
    operator()()
    { this->t_(); delete this;}

    TimerEntry(_T& t)
	: ITimerEntry()
	, t_(t)
    {}
protected:
    _T& t_;
};

template<typename _Class, typename _Arg>
struct MemberTimerEntry : public ITimerEntry
{
    virtual void
    operator()()
    { (this->Class.*Function)(Arg); delete this;}
    
    MemberTimerEntry(_Class& c, void (_Class::*f)(_Arg), const _Arg& a)
	: Class(c)
	, Function(f)
	, Arg(a)
    {}
    
    MemberTimerEntry(_Class& c, void (_Class::*f)(_Arg&), _Arg& a)
	: Class(c)
	, Function(f)
	, Arg(a)
    {}
    virtual ~MemberTimerEntry(){}
protected:
    _Class& Class;
    void (_Class::*Function)(_Arg);
    _Arg Arg;
};

template<typename _Arg>
struct EventTimerEntry : public ITimerEntry
{
    virtual void
    operator()()
    {
	this->Event_(Arg_);
	delete this;
    }

    EventTimerEntry(Event<_Arg>& event, _Arg& arg)
	: ITimerEntry()
	, Event_(event)
	, Arg_(arg)
    {}
protected:
    Event<_Arg>& Event_;
    _Arg Arg_;
};

#endif // SIMULATION__TIMERENTRY_HXX__
