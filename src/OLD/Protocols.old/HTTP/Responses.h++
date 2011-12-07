#ifndef PROTOCOLS__HTTP__RESPONSES_HXX__
#define PROTOCOLS__HTTP__RESPONSES_HXX__

#include "./Response.h++"

namespace Protocols
{
    namespace HTTP
    {
	typedef TSuccessResponse<Response::OK> OK;
	typedef TSuccessResponse<Response::CREATED> Created;
	typedef TSuccessResponse<Response::ACCEPTED> Accepted;
	typedef TSuccessResponse<Response::NOCONTENT> NoContent;

	typedef TRedirectResponse<Response::MOVEDPERMANENTLY> MovedPermanently; 
	typedef TRedirectResponse<Response::FOUND> Found;	
	typedef TRedirectResponse<Response::SEEOTHER> SeeOther;

	typedef TClientErrorResponse<Response::BADREQUEST> BadRequest;	
	typedef TClientErrorResponse<Response::UNAUTHORIZED> Unauthorized;	
	typedef TClientErrorResponse<Response::FORBIDDEN> Forbidden;	
	typedef TClientErrorResponse<Response::NOTFOUND> NotFound;	
	typedef TClientErrorResponse<Response::METHODNOTALLOWED> MethodNotAllowed;
	typedef TClientErrorResponse<Response::REQUESTTIMEOUT> RequestTimeout;
	typedef TClientErrorResponse<Response::ENTITYTOOLARGE> EntityTooLarge;
	typedef TClientErrorResponse<Response::URITOOLONG> URITooLong;	
	typedef TClientErrorResponse<Response::IMATEAPOT> ImATeapot;	

	typedef TServerErrorResponse<Response::INTERNALERROR> InternalServerError;
	typedef TServerErrorResponse<Response::NOTIMPLEMENTED> NotImplemented;
	typedef TServerErrorResponse<Response::SERVICEUNAVAILABLE> ServiceUnavailable;
	typedef TServerErrorResponse<Response::INSUFFICIENTSTORAGE> InsufficientStorage;
	typedef TServerErrorResponse<Response::BANDWIDTHEXCEEDED> BandwidthExceeded;
    }
}

#endif // PROTOCOLS__HTTP__RESPONSES_HXX__
