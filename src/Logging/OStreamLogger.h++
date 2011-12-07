#ifndef LOGGING__OSTREAMLOGGER_HXX__
#define LOGGING__OSTREAMLOGGER_HXX__

#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>

#include "./Logger.h++"

namespace Logging
{
    struct OStreamLogBase;
    typedef Logger<OStreamLogBase> OStreamLogger;
    struct OStreamLogBase
    {
	enum { NEWLINE = '\n' };
	
	OStreamLogBase(const std::string& name)
	    : Name_(name)
	    , DecoratedName_("[" + name + "]: ")
	    , CDecoratedName_(DecoratedName_.data())
	    , DecoratedNameLen_(DecoratedName_.length())
	    , Out_(&std::cout)
	    , Buffer_()
	{}

	virtual ~OStreamLogBase()
	{
	    if(Out_) 
		this->flush();
	}

	std::string
	Name() const 
	{ return this->Name_;}

	void
	Name(const std::string& name)
	{
	    this->Name_ = name;
	    this->DecoratedName_ = "[" + name + "]: ";
	    this->CDecoratedName_ = this->DecoratedName_.data();
	    this->DecoratedNameLen_ = this->DecoratedName_.length();
	}
	
	// std::ostream&
	// Out() const
	// {
	//     return *(this->Out_);
	// }

	// void
	// Out(std::ostream& o)
	// {
	//     this->flush();
	//     o.flush();
	//     this->Out_ = &o;
	// }

	void
	Log(const std::string& s)
	{
	    this->Buffer_ << s;
	}
	
	template<typename _A>
	OStreamLogBase&
	operator<<(const _A& a)
	{
	    this->Buffer_ << a;
	    return *this;
	}

	void
	flush()
	{
	    const std::string buffer = this->Buffer_.str();
	    this->Buffer_.str("");
	    if (buffer.length() == 0) return;
	    const char *p = buffer.data(),
		*pEnd = p + buffer.size(),
		*pCurrent;

	    char newline = (char)(NEWLINE);

	    do
	    {
		Out_->write(this->CDecoratedName_,
			   this->DecoratedNameLen_);
		Out_->put('\t');
		for(pCurrent = p; *p != newline && p < pEnd; ++p);
		Out_->write(pCurrent, ++p - pCurrent);
	    } while (p < pEnd);

	    Out_->flush();
	}

    protected:
	std::string Name_;
	std::string DecoratedName_;
	const char* CDecoratedName_;
	unsigned int DecoratedNameLen_;
	std::ostream* Out_;
	std::ostringstream Buffer_;
    };
    
    struct FileLogBase : public OStreamLogBase
    {
	FileLogBase(const std::string& name)
	    : OStreamLogBase(name)
	    , OutFile_(name + ".log")
	{
	    Out_ = &OutFile_;
	}
	
	~FileLogBase()
	{ 
	    this->flush();
	    OutFile_.close(); Out_ = NULL; 
	}

    protected:
	std::ofstream OutFile_;
    };
    
    typedef Logger<FileLogBase> FileLogger;
} // Logging


#endif // LOGGING__OSTREAMLOGGER_HXX__
