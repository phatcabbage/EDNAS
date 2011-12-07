#ifndef PROTOCOLS__HTTP__SUCCESSRESPONSEMESSAGES_HXX__
#define PROTOCOLS__HTTP__SUCCESSRESPONSEMESSAGES_HXX__

#include <iostream>

#include "./Response.h++"

namespace Protocols
{
    namespace HTTP
    {
	typedef TStatusResponse<Response::SUCCESS> SuccessResponse;

	template<Response::ResponseValue _V>
	struct TSuccessResponse : public SuccessResponse
	{
	    virtual ~TSuccessResponse() {}
	    TSuccessResponse(const URL& uri) 
		: SuccessResponse(_V, uri) {}
	};

	template<>
	struct TSuccessResponse<Response::OK> : public SuccessResponse
	{
	    const Files::File* const
	    Resource; 

	    virtual ~TSuccessResponse() {}
	    
	    TSuccessResponse(const URL& uri,
			     const Files::File* const resource)
		: SuccessResponse(Response::OK, uri)
		, Resource(resource) 
	    {}

	    virtual DataType
	    Size() const
	    { 
		std::cout << "[OKMSG] OK Message has size " << (SuccessResponse::Size() + Resource->Size()) << std::endl;
		return SuccessResponse::Size() + Resource->Size();
	    }
	};

	typedef TSuccessResponse<Response::OK>        OK;
	typedef TSuccessResponse<Response::CREATED>   Created;
	typedef TSuccessResponse<Response::ACCEPTED>  Accepted;
	typedef TSuccessResponse<Response::NOCONTENT> NoContent;
    }
}

#endif // PROTOCOLS__HTTP__SUCCESSRESPONSEMESSAGES_HXX__
