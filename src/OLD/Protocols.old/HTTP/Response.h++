#ifndef PROTOCOLS__HTTP__RESPONSE_HXX__
#define PROTOCOLS__HTTP__RESPONSE_HXX__

#include <string>

#include "./File.h++"
#include "SEDNAS.h++"

namespace Protocols
{
    namespace HTTP
    {
	typedef std::string URL;

	struct Response
	{
	    enum ResponseStatus
	    {
		SUCCESS     = 200,
		REDIRECT    = 300,
		CLIENTERROR = 400,
		SERVERERROR = 500
	    };

	    enum SuccessCode
	    {
		OK        = 200,
		CREATED   = 201,
		ACCEPTED  = 202,
		NOCONTENT = 204,
	    };
	    enum RedirectCode
	    {
		MOVEDPERMANENTLY = 301,
		FOUND            = 302,
		SEEOTHER         = 303,
	    };
	    enum ClientErrorCode
	    {
		BADREQUEST       = 400,
		UNAUTHORIZED     = 401,
		FORBIDDEN        = 402,
		NOTFOUND         = 404,
		METHODNOTALLOWED = 405,
		REQUESTTIMEOUT   = 408,
		ENTITYTOOLARGE   = 413,
		URITOOLONG       = 414,
		IMATEAPOT        = 418,
	    };

	    enum ServerErrorCode
	    {
		INTERNALERROR       = 500,
		NOTIMPLEMENTED      = 501,
		SERVICEUNAVAILABLE  = 503,
		INSUFFICIENTSTORAGE = 507,
		BANDWIDTHEXCEEDED   = 508
	    };

	    const ResponseStatus
	    Status;

	    const int
	    Value;

	    const URL
	    URI;
	    
	    virtual ~Response() {}

	    virtual DataType
	    Size() const
	    { return DataType(4) + DataType(URI.size()); }

	protected:
	    Response(const ResponseStatus status, 
		     const int value, 
		     const URL& uri) 
		: Status(status)
		, Value(value)
		, URI(uri) {}
	};

	template<Response::ResponseStatus _S>
	struct TResponse : public Response
	{
	    virtual ~TResponse() {}

	protected:
	    TResponse(const int value,
		      const URL& uri)
		: Response(_S, value, uri) {}
	};

	typedef TResponse<Response::SUCCESS>     SuccessResponse;
	typedef TResponse<Response::REDIRECT>    RedirectResponse;
	typedef TResponse<Response::CLIENTERROR> ClientErrorResponse;
	typedef TResponse<Response::SERVERERROR> ServerErrorResponse;

	template<Response::SuccessCode _C>
	struct TSuccessResponse : public SuccessResponse
	{
	    TSuccessResponse(const URL& uri)
		: SuccessResponse(_C, uri) {}
	};

	template<Response::RedirectCode _C>
	struct TRedirectResponse : public RedirectResponse
	{
	    const URL& RedirectURI;
	    TRedirectResponse(const URL& uri, const URL& redirectURI)
		: RedirectResponse(_C, uri)
		, RedirectURI(redirectURI) {}
	    
	    virtual DataType
	    Size() const
	    { return Response::Size() + RedirectURI.size();}		
	};

	template<Response::ClientErrorCode _C>
	struct TClientErrorResponse : public ClientErrorResponse
	{
	    TClientErrorResponse(const URL& uri)
		: ClientErrorResponse(_C, uri) {}
	};

	template<Response::ServerErrorCode _C>
	struct TServerErrorResponse : public ServerErrorResponse
	{
	    TServerErrorResponse(const URL& uri)
		: ServerErrorResponse(_C, uri) {}
	};

	template<>
	struct TSuccessResponse<Response::OK> : public SuccessResponse
	{
	    const File* const Resource;
	    TSuccessResponse(const URL& uri, const File* const resource)
		: SuccessResponse(Response::OK, uri)
		, Resource(resource) {}

	    virtual DataType
	    Size() const
	    { return SuccessResponse::Size() + Resource->Size();}
	};
    }
}

#endif // PROTOCOLS__HTTP__RESPONSE_HXX__
