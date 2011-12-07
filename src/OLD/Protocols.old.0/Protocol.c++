#include <iostream>
#include <stdexcept>
#include "./Protocol.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Protocols::Protocol>::
    Logger("Protocol");
}

namespace Protocols
{
    const char*
    Protocol::
    DEFAULT_NAME = "Unnamed Protocol";

    Protocol::ProtocolList
    Protocol::
    G_Protocols;

    Protocol::ProtocolNameMap
    Protocol::
    G_ProtocolByName;

    Protocol::
    Protocol(const std::string name,
	     const double addressSize,
	     const double headerSize,
	     const double footerSize)
	: Index_(Protocol::G_Protocols.size())
	, Name_(name)
	, Contained_()
	, Container_(Index_)
    {
	Logger << Logging::Debug << "Created \""
	       << this->Name_ << "\"" << Logging::endl;
    }

    Protocol&
    Protocol::Create(const std::string& name)
    {
	return Protocol::Get(name);
    }

    Protocol&
    Protocol::Create(const std::string& name,
		     Protocol& container)
    {
	Protocol& p(Protocol::Get(name));
	container += p;
	return p;
    }

    Protocol&
    Protocol::
    Get(const std::string& name)
    {
	if (!Protocol::G_ProtocolByName.count(name))
	{
	    Protocol p(name);
	    Register(p);
	}
	
	return Protocol::Get(Protocol::G_ProtocolByName[name]);
    }
    
    void
    Protocol::
    Register(Protocol& p)
    {
	Logger << Logging::Debug << "Registering \""
	       << p.Name_ << "\"" << Logging::endl;

	Protocol::G_Protocols.push_back(p);
	Protocol& newp = Protocol::G_Protocols.back();
	newp.Index_ = Protocol::G_Protocols.size() -1;
	Protocol::G_ProtocolByName[newp.Name()] = newp.Index();
    }

    Protocol&
    Protocol::
    operator+=(Protocol& p)
    {
	if (p.Container_ != p.Index())
	{
	    Protocol& oldContainer = p.Container();
	    oldContainer.Contained_.erase(p.Index());
	}
	p.Container_ = this->Index();
	
	this->Contained_.insert(p.Index());
	return *this;
    }

    std::ostream&
    operator<<(std::ostream& o, const Protocol& p)
    {
	o << "[Protocol: \"" << p.Name() << "\""
	  << " (container:";
	if (p.Container() == p)
	    o << "self";
	else o << "\"" << p.Container().Name() << "\"";
	
	o << ")]";
	return o;
    }
}
