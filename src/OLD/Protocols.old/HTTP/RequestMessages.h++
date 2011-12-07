#ifndef PROTOCOLS__HTTP__REQUESTMESSAGES_HXX__
#define PROTOCOLS__HTTP__REQUESTMESSAGES_HXX__

#include <string>

#include "Foundation/Network/Message.h++"
#include "SEDNAS.h++"
#include "./File.h++"

namespace Protocols
{
    namespace HTTP
    {
	typedef std::string URL;

	struct Request
	{
	    const URL
	    URI;

	    virtual ~Request() {}
	    
	    virtual DataType
	    Size() const
	    { return DataType(URI.size()); }

	protected:
	    Request(const URL& uri)
		: URI(uri) {}
	};

	struct Get : public Request
	{
	    Get(const URL& uri)
		: Request(uri) {}

	    virtual ~Get() {}
	};

	struct Put : public Request
	{
	    const File* const
	    Resource;

	    Put(const URL& uri,
		const File* const resource)
		: Request(uri)
		, Resource(resource)
	    {}
	    
	    virtual ~Put() {}

	    virtual DataType
	    Size() const
	    { return (Request::Size() + Resource->Size()); }
	};

	struct Delete : public Request
	{
	    Delete(const URL& uri)
		: Request(uri) {}

	    virtual ~Delete() {}
	};

	typedef Foundation::Net::Message<Get>    GetMessage;
	typedef Foundation::Net::Message<Put>    PutMessage;
	typedef Foundation::Net::Message<Delete> DeleteMessage;
    }
}

#endif // PROTOCOLS__HTTP__REQUESTMESSAGES_HXX__
