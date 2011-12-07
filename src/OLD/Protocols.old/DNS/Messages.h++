#ifndef PROTOCOLS__DNS__SERVER_HXX__
#define PROTOCOLS__DNS__SERVER_HXX__

#include <string>

#include "Network/Message.h++"
#include "SEDNAS.h++"

namespace Protocols
{
    namespace DNS
    {
	struct Query
	{
	    const std::string 
	    Name;

	    Query(const std::string& name) 
		: Name(name) {}

	    ~Query() {}
	    
	    DataType
	    Size() const
	    { return Name.size(); }
	};

	struct Answer
	{
	    const std::string
	    Name;

	    const Net::NodeAddress
	    Address;

	    Answer(const std::string& name,
		   const Net::NodeAddress& address) 
		: Name(name)
		, Address(address) {}

	    Answer(const std::string& name) 
		: Name(name)
		, Address(0) {}

	    virtual ~Answer() {}
	    
	    DataType
	    Size() const
	    { return Name.size() + DataType(Net::Network::ADDRESS_LEN); }
	};

	struct Update
	{
	    const std::string 
	    Name;

	    const Net::NodeAddress
	    Address;

	    Update(const Net::NodeAddress address,
		   const std::string& name) 
		: Name(name)
		, Address(address) {}

	    ~Update() {}

	    DataType
	    Size() const
	    { return Name.size() + DataType(Net::Network::ADDRESS_LEN); }
	};

	typedef Net::Message<Query> QueryMessage;
	typedef Net::Message<Answer> AnswerMessage;
	typedef Net::Message<Update> UpdateMessage;
    }
}

#endif // PROTOCOLS__DNS__SERVER_HXX__
