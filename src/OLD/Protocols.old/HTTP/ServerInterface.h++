#ifndef PROTOCOLS__HTTP__SERVERINTERFACE_HXX__
#define PROTOCOLS__HTTP__SERVERINTERFACE_HXX__

#include "./Messages.h++"

namespace Protocols
{
    namespace HTTP
    {
	struct ServerSuccessMessageInterface;
	struct ServerRedirectMessageInterface;
	struct ServerClientErrorMessageInterface;
	struct ServerServerErrorMessageInterface;

	struct ServerResponseInterface;
	struct ServerRequestInterface;

	struct ServerInterface;

	struct ServerSuccessMessageInterface
	    : public OKMessage::SenderType
	    , public CreatedMessage::SenderType
	    , public AcceptedMessage::SenderType
	    , public NoContentMessage::SenderType
	{
	    virtual void 
	    Sent(OKMessage* const msg) = 0;

	    virtual 
	    ~ServerSuccessMessageInterface(){}
	};

	struct ServerRedirectMessageInterface
	    : public MovedPermanentlyMessage::SenderType
	    , public FoundMessage::SenderType
	    , public SeeOtherMessage::SenderType
	{
	    virtual void 
	    Sent(MovedPermanentlyMessage* const msg) = 0;

	    virtual ~ServerRedirectMessageInterface(){}
	};

	struct ServerClientErrorMessageInterface
	    : public BadRequestMessage::SenderType
	    , public UnauthorizedMessage::SenderType
	    , public ForbiddenMessage::SenderType
	    , public NotFoundMessage::SenderType
	    , public MethodNotAllowedMessage::SenderType
	    , public RequestTimeoutMessage::SenderType
	    , public EntityTooLargeMessage::SenderType
	    , public URITooLongMessage::SenderType
	    , public ImATeapotMessage::SenderType
	{
	    virtual void
	    Sent(BadRequestMessage* const msg) = 0;
	    virtual ~ServerClientErrorMessageInterface(){}
	};

	struct ServerServerErrorMessageInterface
	    : public InternalServerErrorMessage::SenderType
	    , public NotImplementedMessage::SenderType
	    , public ServiceUnavailableMessage::SenderType
	    , public InsufficientStorageMessage::SenderType
	    , public BandwidthExceededMessage::SenderType
	{
	    virtual void
	    Sent(InternalServerErrorMessage* const msg) = 0;

	    virtual ~ServerServerErrorMessageInterface(){}
	};

	struct ServerResponseInterface
	    : public ServerSuccessMessageInterface
	    , public ServerRedirectMessageInterface
	    , public ServerClientErrorMessageInterface
	    , public ServerServerErrorMessageInterface
	{
	    virtual ~ServerResponseInterface(){}
	};

	struct ServerRequestInterface
	    : public GetMessage::ReceiverType
	    , public PutMessage::ReceiverType
	    , public DeleteMessage::ReceiverType
	{
	    virtual void
	    Received(GetMessage* const msg) = 0;

	    virtual ~ServerRequestInterface(){}
	};
	    
	struct ServerInterface
	    : public ServerRequestInterface
	    , public ServerResponseInterface
	{
	    virtual void
	    Received(GetMessage* const msg) = 0;
	    
	    virtual void
	    Received(PutMessage* const msg) = 0;

	    virtual void
	    Received(DeleteMessage* const msg) = 0;

	    virtual void
	    Sent(OKMessage* const msg){}
	    
	    virtual void
	    Sent(MovedPermanentlyMessage* const msg){}

	    virtual void
	    Sent(BadRequestMessage* const msg){}

	    virtual void
	    Sent(InternalServerErrorMessage* const msg){}
	    
	    virtual ~ServerInterface(){}

	protected:
	    ServerInterface(){}
	};
    }
}

#endif // PROTOCOLS__HTTP__SERVERINTERFACE_HXX__
