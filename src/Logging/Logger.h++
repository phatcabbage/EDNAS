#ifndef LOG_HXX__
#define LOG_HXX__

#include <string>

// #include "Core/Numerical/DataTypes.h++" // for operator<<(Logger&, DataType&)

namespace Logging
{
    enum LogLevel
    {
	None,
	Debug,
	Normal,
	Warning,
	Error,
	Fatal,
	All
    };

    struct NullLogBase;

    template<typename _LogBase = NullLogBase>
    struct Logger
    {
	typedef Logger& (*Manipulator)(Logger&);

	_LogBase Base;

	LogLevel
	DefaultLogLevel() const
	{ return Default;}

	LogLevel
	CurrentLogLevel() const
	{ return Current;}

	LogLevel
	SuppressLogLevel() const
	{ return Suppress; }
	
	void
	DefaultLogLevel(LogLevel l)
	{ Default = l; }
	
	void
	CurrentLogLevel(LogLevel l)
	{ Current = l; }
	
	void
	SuppressLogLevel(LogLevel l)
	{ Suppress = l; }


	void 
	Log(const std::string& s)
	{ if (Current > Suppress) Base.Log(s + "\n"); Base.flush();}

	void 
	Log(const LogLevel& level, const std::string& s)
	{ if (level > Suppress) Base.Log(s + "\n"); Base.flush();}

	template<typename _A>
	Logger&
	operator<<(const _A& a)
	{ 
	    if (Current > Suppress)
		Base << a;
	    return *this;
	}

	void 
	flush()
	{
	    Base.flush();
	    CurrentLogLevel(Default);
	}

	Logger&
	operator<<(Logger& (*manip)(Logger&))
	{
	    manip(*this);
	    return *this;
	}

	Logger&
	operator<<(LogLevel level)
	{
	    CurrentLogLevel(level);
	    return *this;
	}

	Logger(const std::string& name, 
	       LogLevel suppress = None,
	       LogLevel current = Normal)
	    : Base(name)
	    , Default(Normal)
	    , Suppress(suppress)
	    , Current(current)
	{
	    this->Log(Debug, "Logger Created.");
	}
	
	~Logger()
	{ this->Log(Debug, "Logger Destroyed."); }

	template<LogLevel _Lev>
	static
	Logger&
	SetLogLevel(Logger& l)
	{
	    l.CurrentLogLevel(_Lev);
	    return l;
	}

    protected:
	LogLevel Default;
	LogLevel Suppress;
	LogLevel Current;

    private:
	Logger(const Logger& log);
    };

    struct NullLogBase
    {
	void
	flush();

	void
	Log(const std::string& s){}

	template<typename _A>
	NullLogBase&
	operator<<(const _A& a)
	{ return *this; }
	
	NullLogBase(const std::string& name){}
    };

    template<typename _T>
    Logger<_T>&
    endl(Logger<_T>& l)
    { l << "\n"; l.flush(); return l;}

    template<typename _T>
    Logger<_T>&
    flush(Logger<_T>& l)
    { l.flush(); return l; }

//     template<typename _L, int _T, int _D>
//     Logger<_L>&
//     operator<<(Logger<_L>& l, const Core::Numerical::DataType<_T,_D>& d)
//     { l << d.Value(); }
} // Logging

#endif // LOG_HXX__
