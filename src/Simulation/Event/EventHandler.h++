#ifndef CORE__SIGNALING__EVENTHANDLER_HXX__
#define CORE__SIGNALING__EVENTHANDLER_HXX__

template<typename _EArgs>
class EventHandler;

template<typename _EArgs>
class EventHandler
{
public:
    typedef _EArgs ArgType;

    virtual void
    operator()(ArgType arg) = 0;
};

// template<>
// class EventHandler<void>
// {
// public:
//     typedef void ArgType;
    
//     virtual void
//     operator()() = 0;
// };

#endif // CORE__SIGNALING__EVENTHANDLER_HXX__
