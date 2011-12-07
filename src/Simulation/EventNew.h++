#ifndef EVENT__EVENT_HXX__
#define EVENT__EVENT_HXX__

#include <vector>

template<typename _Arg>
struct Event;

template<typename _Arg>
struct EventHandler
{
    /** 
	Fires the event.
	@param Arg The argument to be passed to handlers.
    */
    virtual void
    operator()(_Arg arg) = 0;

    virtual ~EventHandler() {}
protected:
    virtual void
    Associated(Event& e) = 0;
    
    virtual void
    Dissociated(Event& e) = 0;
};

template<typename _Arg>
struct Event : public EventHandler<_Arg>
{
    virtual operator bool() const = 0;

    virtual Event&
    operator+=(EventHandler& eh) = 0;
    
    virtual Event&
    operator-=(EventHandler& eh) = 0;
    
    virtual ~Event()
    { this->DissociateHandlers(); }

protected:
    virtual void
    DissociateHandlers() = 0;
};

template<typename _DelegateClass, typename _Arg>
struct EventAdapter : public EventHandler<_Arg>
{
private:
    typedef void (_DelegateClass::*HandlerFunction)(_Arg);

    _DelegateClass& Delegate_;
    HandlerFunction Handler_;
    Event<_Arg>* Event_;

protected:
    void 
    Associated(Event<_Arg>& e)
    {
	if (this->Event_ && &e != this->Event_)
	    *(this->Event_) -= *this;
	this->Event_ = &e;
    }

    void 
    Dissociated(const Event<_Arg>& e)
    {
	if (this->Event_ && &e == this->Event_)
	    this->Event_ = NULL;
    }

public:
    EventAdapter(DelegateType& delegate, HandlerFunction handler)
	: Delegate_(delegate_),
	  Handler_(handler),
	  Event_(NULL)
    {}

    void
    Dissociate()
    {	if (this->Event_) *(this->Event_) -= this;    }
    
    virtual ~EventAdapter() {}
    { this->Dissociate(); }
};

template<typename _EArg>
struct SingleEvent : public Event<_EArg>
{
    SingleEvent() : Handler_(NULL){}
    virtual SingleEvent() {}

    SingleEvent&
    operator+=(Handler& handler)
    { 
	
    }

    SingleEvent&
    operator-=(const Handler& handler)
    { return (*this -= &handler); }

protected:
    EventHandler<_Arg>* 
    Handler_;
};
/* continue inputting event.h++.... */

#endif // EVENT__EVENT_HXX__
