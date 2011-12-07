#ifndef PROTOCOLS__HTTP__RESPONSEMESSAGES_HXX__
#define PROTOCOLS__HTTP__RESPONSEMESSAGES_HXX__

#include "./Responses.h++"
#include "Foundation/Network/Message.h++"
#include "SEDNAS.h++"

namespace Protocols
{
    namespace HTTP
    {
	typedef Foundation::Net::Message<SuccessResponse> SuccessMessage;
	typedef Foundation::Net::Message<OK>        OKMessage;
	typedef Foundation::Net::Message<Created>   CreatedMessage;
	typedef Foundation::Net::Message<Accepted>  AcceptedMessage;
	typedef Foundation::Net::Message<NoContent> NoContentMessage;

	typedef Foundation::Net::Message<RedirectResponse> RedirectMessage;
	typedef Foundation::Net::Message<MovedPermanently> MovedPermanentlyMessage;
	typedef Foundation::Net::Message<Found>            FoundMessage;
	typedef Foundation::Net::Message<SeeOther>         SeeOtherMessage;

	typedef Foundation::Net::Message<ClientErrorResponse> ClientErrorMessage;
	typedef Foundation::Net::Message<BadRequest>       BadRequestMessage;
	typedef Foundation::Net::Message<Unauthorized>     UnauthorizedMessage;
	typedef Foundation::Net::Message<Forbidden>        ForbiddenMessage;
	typedef Foundation::Net::Message<NotFound>         NotFoundMessage;
	typedef Foundation::Net::Message<MethodNotAllowed> MethodNotAllowedMessage;
	typedef Foundation::Net::Message<RequestTimeout>   RequestTimeoutMessage;
	typedef Foundation::Net::Message<EntityTooLarge>   EntityTooLargeMessage;
	typedef Foundation::Net::Message<URITooLong>       URITooLongMessage;
	typedef Foundation::Net::Message<ImATeapot>        ImATeapotMessage;

	typedef Foundation::Net::Message<ServerErrorResponse> ServerErrorMessage;
	typedef Foundation::Net::Message<InternalServerError> InternalServerErrorMessage;
	typedef Foundation::Net::Message<NotImplemented>      NotImplementedMessage;
	typedef Foundation::Net::Message<ServiceUnavailable>  ServiceUnavailableMessage;
	typedef Foundation::Net::Message<InsufficientStorage> InsufficientStorageMessage;
	typedef Foundation::Net::Message<BandwidthExceeded>   BandwidthExceededMessage;
    }
}

#endif // PROTOCOLS__HTTP__RESPONSEMESSAGES_HXX__
