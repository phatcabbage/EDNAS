#ifndef LOGGING__ILOGGABLE_HXX__
#define LOGGING__ILOGGABLE_HXX__

#include "./Logging.h++"

namespace Logger
{
    struct ILoggable
    {
	string Name;
	ILoggable(const string& name) : Name(name) {}
    };
}

#endif // LOGGING__ILOGGABLE_HXX__
