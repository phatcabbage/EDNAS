#ifndef EVENTIMPL_HXX__
#define EVENTIMPL_HXX__

template<typename _EArg>
class Event : public EventHandler<_EArg>
{
public:
    typedef _EArg ArgType;
    typedef EventHandler<ArgType> Handler;

    virtual void operator()(ArgType) = 0;
    virtual Event& operator+=(Handler& handler) = 0;
    virtual Event& operator-=(const Handler& handler) = 0;
};

// template<>
// class Event<void> : public EventHandler<void>
// {
// public:
//     typedef void ArgType;
//     typedef EventHandler<void> Handler;

//     virtual void operator()() = 0;
// };

#endif // EVENTIMPL_HXX__
