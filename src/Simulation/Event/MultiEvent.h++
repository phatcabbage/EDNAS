#ifndef CORE__SIGNALING__MULTIEVENT_HXX__
#define CORE__SIGNALING__MULTIEVENT_HXX__

template<typename _EArg>
class MultiEvent : public Event<_EArg>
{
public:
    typedef _EArg ArgType;

private:
    typedef EventHandler<ArgType> Handler;
    typedef std::vector<Handler*> HandlerContainer;

    HandlerContainer
    Handlers_;
    
    // MultiEvent& 
    // operator=(const MultiEvent& handler)

    // MultiEvent(const MultiEvent& o)
    // {

protected:
    void 
    Fire(ArgType arg)
    { 
	for (typename HandlerContainer::iterator 
		 it = this->Handlers_.begin(),
		 end = this->Handlers_.end();
	     it != end; 
	     ++it)
	{
	    (*(*it))(arg);
	}
    }

public:
    MultiEvent()
	: Event<ArgType>()
	, Handlers_() {}

    virtual ~MultiEvent() {}

    void 
    operator()(ArgType arg)
    {
	this->Fire(arg);
    }

    MultiEvent&
    operator+=(Handler* handler) 
    { 
	if (handler == this) return *this;
	if (std::count(this->Handlers_.begin(), this->Handlers_.end(), handler) == 0)
	    this->Handlers_.push_back(handler);
	return *this;
    }

    MultiEvent&
    operator+=(Handler& handler)
    { 
	if (&handler == this) return *this;
	if (std::count(this->Handlers_.begin(), this->Handlers_.end(), &handler) == 0)
	    this->Handlers_.push_back(&handler);
	return *this;
    }

    // template<typename _Class>
    // MultiEvent&
    // operator+=(void (_Class::fn)(ArgType&))
    // {
    // 	this->Handlers_.push_back(new EventAdapter<_Class>

    MultiEvent&
    operator-=(const Handler* const handler) 
    { 
	if (this->Handlers_.size() == 0) return *this;

	typename HandlerContainer::iterator found = 
	    std::find(this->Handlers_.begin(),
		      this->Handlers_.end(),
		      handler);

	if (found != this->Handlers_.end())
	{
	    if (*found != this->Handlers_.back())
		*found = this->Handlers_.back();
	    this->Handlers_.pop_back();
	}

	return *this;
    }

    MultiEvent&
    operator -=(const Handler& handler)
    { return (*this -= &handler); }
};

// template<>
// class MultiEvent<void> : public EventHandler<void>
// {
// public:
//     typedef void ArgType;

// private:
//     typedef EventHandler<ArgType> Handler;
//     typedef std::vector<Handler*> HandlerContainer;

//     HandlerContainer
//     Handlers_;
    
//     MultiEvent& 
//     operator=(const MultiEvent& handler);

//     MultiEvent(const MultiEvent& o);

// protected:
//     void 
//     Fire()
//     { 
// 	for (HandlerContainer::iterator 
// 		 it = this->Handlers_.begin(),
// 		 end = this->Handlers_.end();
// 	     it != end; 
// 	     ++it)
// 	{
// 	    (*(*it))();
// 	}
//     }

// public:
//     MultiEvent()
// 	: EventHandler<ArgType>()
// 	, Handlers_() {}

//     virtual ~MultiEvent() {}

//     void 
//     operator()()
//     {
// 	this->Fire();
//     }

//     MultiEvent&
//     operator+=(Handler* handler) 
//     {
// 	if (handler == this) return *this;
// 	if (std::count(this->Handlers_.begin(), this->Handlers_.end(), handler) == 0)
// 	    this->Handlers_.push_back(handler);
// 	return *this;
//     }

//     MultiEvent&
//     operator+=(Handler& handler)
//     { 
// 	if (&handler == this) return *this;
// 	if (std::count(this->Handlers_.begin(), this->Handlers_.end(), &handler) == 0)
// 	    this->Handlers_.push_back(&handler);
// 	return *this;
//     }


//     MultiEvent&
//     operator-=(const Handler* const handler) 
//     { 
// 	if (this->Handlers_.size() == 0) return *this;

// 	HandlerContainer::iterator found = 
// 	    std::find(this->Handlers_.begin(),
// 		      this->Handlers_.end(),
// 		      handler);

// 	if (found != this->Handlers_.end())
// 	{
// 	    if (*found != this->Handlers_.back())
// 		*found = this->Handlers_.back();
// 	    this->Handlers_.pop_back();
// 	}

// 	return *this;
//     }

//     MultiEvent&
//     operator -=(const Handler& handler)
//     { return (*this -= &handler); }
// };

#endif // CORE__SIGNALING__MULTIEVENT_HXX__
