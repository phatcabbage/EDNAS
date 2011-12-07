#ifndef PROTOCOLS__PROTOCOL_HXX__
#define PROTOCOLS__PROTOCOL_HXX__

#include <iosfwd>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Logging/Logged.h++"

namespace Protocols
{
    struct Protocol : public Logging::Logged<Protocol>
    {
	typedef unsigned int ProtocolIndex;

	typedef std::vector<Protocol>
	ProtocolList;

	typedef std::map<std::string, ProtocolIndex>
	ProtocolNameMap;

	typedef std::set<ProtocolIndex>
	ProtocolSet;

	const static char* DEFAULT_NAME; // = "Unnamed";
	const static int DEFAULT_ADDRESS_SIZE = 32;
	const static int DEFAULT_HEADER_SIZE = 2 * DEFAULT_ADDRESS_SIZE;
	const static int DEFAULT_FOOTER_SIZE = 0;

	static const ProtocolList&
	Protocols()
	{ return Protocol::G_Protocols; }
	
	static Protocol&
	Create(const std::string& name);

	static Protocol&
	Create(const std::string& name, 
	       Protocol& container);

	static Protocol&
	Get(ProtocolIndex i)
	{ return Protocol::G_Protocols[i];}

	static Protocol&
	Get(const std::string& name);

	// Non-static

	std::string 
	Name() const
	{ return this->Name_; }
	
	ProtocolIndex
	Index() const
	{ return this->Index_; }

	const ProtocolSet&
	Contained() const
	{ return this->Contained_; }

	Protocol&
	Container() const
	{ return Protocol::G_Protocols[this->Container_]; }

	double 
	AddressSize() const
	{ return this->AddressSize_;}
	
	double
	HeaderSize() const
	{ return this->HeaderSize_;}

	double
	FooterSize() const
	{ return this->FooterSize_;}

	void
	Name(const std::string& name)
	{ this->Name_ = name; }
	
	void
	AddressSize(const double& size)
	{ this->AddressSize_ = size; }

	void
	HeaderSize(const double& size)
	{ this->HeaderSize_ = size; }
	
	void
	FooterSize(const double& size)
	{ this->FooterSize_ = size; }

	Protocol&
	operator+=(Protocol& p);

	~Protocol() {}

	bool
	operator==(const Protocol& p) const
	{ return this == &p; }

    private:
	Protocol(const std::string name   = DEFAULT_NAME,
		 const double addressSize = DEFAULT_ADDRESS_SIZE,
		 const double headerSize  = DEFAULT_HEADER_SIZE,
		 const double footerSize  = DEFAULT_FOOTER_SIZE);

	static ProtocolList
	G_Protocols;

	static ProtocolNameMap
	G_ProtocolByName;

	static void
	Register(Protocol& p);

	ProtocolIndex
	Index_;

	std::string
	Name_;

	ProtocolSet
	Contained_;

	ProtocolIndex
	Container_;

	double
	AddressSize_;
	
	double
	HeaderSize_;
	
	double
	FooterSize_;
    };

    std::ostream&
    operator<<(std::ostream&, const Protocol& p);
}

#endif // PROTOCOLS__PROTOCOL_HXX__
