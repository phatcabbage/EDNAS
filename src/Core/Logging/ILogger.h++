#ifndef LOGGING__ILOGGER_HXX__
#define LOGGING__ILOGGER_HXX__

#include "./Logging.h++"

namespace Logging
{
    struct ILogger
    {
	const ILoggable* const 
	Loggable;

	ILogger(const ILogger* const loggable) : Loggable(loggable) {}

	void
	Log(const string& s) = 0;

	template<typename _Key, typename _Value>
	void
	LogValue(const _Key& key, const _Value& value) = 0;
    };
}

#endif
