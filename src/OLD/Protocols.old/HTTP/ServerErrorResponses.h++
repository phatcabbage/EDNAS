#ifndef PROTOCOLS__HTTP__SERVERERRORRESPONSES_HXX__
#define PROTOCOLS__HTTP__SERVERERRORRESPONSES_HXX__

#include "./Response.h++"

namespace Protocols
{
    namespace HTTP
    {
	typedef TStatusResponse<Response::SERVERERROR> ServerErrorResponse;

	template<Response::ResponseValue _V>
	struct TServerErrorResponse : public ServerErrorResponse
	{
	    
	    virtual ~TServerErrorResponse() {}
	    TServerErrorResponse(const URL& uri) 
		: ServerErrorResponse(_V, uri) {}
	};

	typedef TServerErrorResponse<Response::INTERNALERROR>       InternalServerError;
	typedef TServerErrorResponse<Response::NOTIMPLEMENTED>      NotImplemented;
	typedef TServerErrorResponse<Response::SERVICEUNAVAILABLE>  ServiceUnavailable;
	typedef TServerErrorResponse<Response::INSUFFICIENTSTORAGE> InsufficientStorage;
	typedef TServerErrorResponse<Response::BANDWIDTHEXCEEDED>   BandwidthExceeded;
    }
}

#endif // PROTOCOLS__HTTP__SERVERERRORRESPONSES_HXX__
