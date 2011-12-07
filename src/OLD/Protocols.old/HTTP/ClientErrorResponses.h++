#ifndef PROTOCOLS__HTTP__CLIENTERRORRESPONSES_HXX__
#define PROTOCOLS__HTTP__CLIENTERRORRESPONSES_HXX__

#include "./Response.h++"

namespace Protocols
{
    namespace HTTP
    {
 	typedef TStatusResponse<Response::CLIENTERROR> ClientErrorResponse;

	template<Response::ResponseValue _V>
	struct TClientErrorResponse : public ClientErrorResponse
	{
	    
	    virtual ~TClientErrorResponse() {}
	    TClientErrorResponse(const URL& uri) 
		: ClientErrorResponse(_V, uri) {}
	};

	typedef TClientErrorResponse<Response::BADREQUEST>       BadRequest;
	typedef TClientErrorResponse<Response::UNAUTHORIZED>     Unauthorized;
	typedef TClientErrorResponse<Response::FORBIDDEN>        Forbidden;
	typedef TClientErrorResponse<Response::NOTFOUND>         NotFound;
	typedef TClientErrorResponse<Response::METHODNOTALLOWED> MethodNotAllowed;
	typedef TClientErrorResponse<Response::REQUESTTIMEOUT>   RequestTimeout;
	typedef TClientErrorResponse<Response::ENTITYTOOLARGE>   EntityTooLarge;
	typedef TClientErrorResponse<Response::URITOOLONG>       URITooLong;
	typedef TClientErrorResponse<Response::IMATEAPOT>        ImATeapot;
    }
}

#endif // PROTOCOLS__HTTP__CLIENTERRORRESPONSES_HXX__
