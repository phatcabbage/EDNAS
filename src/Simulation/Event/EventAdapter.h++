#ifndef CORE__SIGNALING__EVENTADAPTER_HXX__
#define CORE__SIGNALING__EVENTADAPTER_HXX__

template<typename _DType, typename _EArgs>
class EventAdapter;

template <typename _DType, typename _EArg>
class EventAdapter : public EventHandler<_EArg>
{
public:
    typedef _DType DelegateType;
    typedef _EArg  EventArgType;
    typedef void   (DelegateType::*HandlerFunction)(EventArgType);

private:
    DelegateType&
    Delegate_;
    
    HandlerFunction
    Handler_;

public:

    EventAdapter(DelegateType& delegate, HandlerFunction handler)
	: Delegate_(delegate)
	, Handler_(handler)
    {}

    void
    operator()(EventArgType arg)
    { (Delegate_.*Handler_)(arg);}

    bool
    operator==(const EventAdapter& other) const
    { 
	return ((Delegate_ == other.Delegate_)&& 
	      (Handler_ == other.Handler_));
    }
};

// template <typename _DType>
// class EventAdapter<_DType,void> : public EventHandler<void>
// {
// public:
//     typedef _DType DelegateType;
//     typedef void   (DelegateType::*HandlerFunction)();

// private:
//     DelegateType&
//     Delegate_;
    
//     HandlerFunction
//     Handler_;

// public:
//     EventAdapter(DelegateType& delegate, HandlerFunction handler)
// 	: Delegate_(delegate)
// 	, Handler_(handler)
//     {}

//     void
//     operator()()
//     { (Delegate_.*Handler_)();}

//     bool
//     operator==(const EventAdapter& other) const
//     { 
// 	return ((Delegate_ == other.Delegate_)&& 
// 	      (Handler_ == other.Handler_));
//     }
// };

template<typename _D, typename _Arg>
EventAdapter<_D,_Arg>*
CreateAdapter(_D& d, void (_D::*fn)(_Arg))
{
    return new EventAdapter<_D,_Arg>(d, fn);
}

#endif // CORE__SIGNALING__EVENTADAPTER_HXX__
