#ifndef PROTOCOLS__HTTP__CLIENTINTERFACE_HXX__
#define PROTOCOLS__HTTP__CLIENTINTERFACE_HXX__

#include "./Messages.h++"

namespace Protocols
{
    namespace HTTP
    {
	struct ClientSuccessMessageInterface;
	struct ClientRedirectMessageInterface;
	struct ClientClientErrorMessageInterface;
	struct ClientServerErrorMessageInterface;

	struct ClientResponseInterface;
	struct ClientRequestInterface;

	struct ClientInterface;

	struct ClientSuccessMessageInterface
	    : public OKMessage::ReceiverType
	    , public CreatedMessage::ReceiverType
	    , public AcceptedMessage::ReceiverType
	    , public NoContentMessage::ReceiverType
	{
	    virtual ~ClientSuccessMessageInterface(){}
	};

	struct ClientRedirectMessageInterface
	    : public MovedPermanentlyMessage::ReceiverType
	    , public FoundMessage::ReceiverType
	    , public SeeOtherMessage::ReceiverType
	{
	    virtual ~ClientRedirectMessageInterface(){}
	};

	struct ClientClientErrorMessageInterface
	    : public BadRequestMessage::ReceiverType
	    , public UnauthorizedMessage::ReceiverType
	    , public ForbiddenMessage::ReceiverType
	    , public NotFoundMessage::ReceiverType
	    , public MethodNotAllowedMessage::ReceiverType
	    , public RequestTimeoutMessage::ReceiverType
	    , public EntityTooLargeMessage::ReceiverType
	    , public URITooLongMessage::ReceiverType
	    , public ImATeapotMessage::ReceiverType
	{
	    virtual ~ClientClientErrorMessageInterface(){}
	};

	struct ClientServerErrorMessageInterface
	    : public InternalServerErrorMessage::ReceiverType
	    , public NotImplementedMessage::ReceiverType
	    , public ServiceUnavailableMessage::ReceiverType
	    , public InsufficientStorageMessage::ReceiverType
	    , public BandwidthExceededMessage::ReceiverType
	{
	    virtual ~ClientServerErrorMessageInterface(){}
	};

	struct ClientResponseInterface
	    : public ClientSuccessMessageInterface
	    , public ClientRedirectMessageInterface
	    , public ClientClientErrorMessageInterface
	    , public ClientServerErrorMessageInterface
	{
	    virtual ~ClientResponseInterface(){}
	};

	struct ClientRequestInterface
	    : public GetMessage::SenderType
	    , public PutMessage::SenderType
	    , public DeleteMessage::SenderType
	{
	    virtual ~ClientRequestInterface(){}
	};
	    
	struct ClientInterface
	    : public ClientRequestInterface
	    , public ClientResponseInterface
	{
	    virtual void Received(OKMessage* const msg) { }
	    virtual void Received(CreatedMessage* const msg) { }
	    virtual void Received(AcceptedMessage* const msg) { }
	    virtual void Received(NoContentMessage* const msg) { }
	    virtual void Received(MovedPermanentlyMessage* const msg) { }
	    virtual void Received(FoundMessage* const msg) { }
	    virtual void Received(SeeOtherMessage* const msg) { }
	    virtual void Received(BadRequestMessage* const msg) { }
	    virtual void Received(UnauthorizedMessage* const msg) { }
	    virtual void Received(ForbiddenMessage* const msg) { }
	    virtual void Received(NotFoundMessage* const msg) { }
	    virtual void Received(MethodNotAllowedMessage* const msg) { }
	    virtual void Received(RequestTimeoutMessage* const msg) { }
	    virtual void Received(EntityTooLargeMessage* const msg) { }
	    virtual void Received(URITooLongMessage* const msg) { }
	    virtual void Received(ImATeapotMessage* const msg) { }
	    virtual void Received(InternalServerErrorMessage* const msg) { }
	    virtual void Received(NotImplementedMessage* const msg) { }
	    virtual void Received(ServiceUnavailableMessage* const msg) { }
	    virtual void Received(InsufficientStorageMessage* const msg) { }
	    virtual void Received(BandwidthExceededMessage* const msg) { }
	    
	    virtual ~ClientInterface(){}
	    
	protected:
	    ClientInterface(){}
	};
    }
}

#endif // PROTOCOLS__HTTP__CLIENTINTERFACE_HXX__
