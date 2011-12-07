#ifndef PROTOCOLS__DNS__ACTORS_HXX__
#define PROTOCOLS__DNS__ACTORS_HXX__

#include "SEDNAS.h++"
#include "Network/Node.h++"

namespace Protocols
{
    namespace DNS
    {
	struct ServerInterface 
	    : public QueryMessage::ReceiverType
	    , public AnswerMessage::SenderType
	    , public UpdateMessage::ReceiverType
	{
	    ServerInterface()
		: QueryMessage::ReceiverType()
		, AnswerMessage::SenderType()
		, QueryMessage::ReceiverType() {}

	    virtual ~ServerInterface() {}

	    virtual void
	    Received(QueryMessage* const msg) = 0;

	    virtual void
	    Sent(AnswerMessage* const msg) {}
	    
	    virtual void
	    Received(UpdateMessage* const msg) {}
	};

	struct QuerierInterface
	    : public QueryMessage::SenderType
	    , public AnswerMessage::ReceiverType
	{
	    QuerierInterface()
		: QueryMessage::SenderType()
		, AnswerMessage::ReceiverType() {}

	    virtual ~QuerierInterface() {}

	    virtual void
	    Sent(QueryMessage* const msg) {}

	    virtual void
	    Received(AnswerMessage* const msg) = 0;
	};

	struct HostInterface
	    : public QuerierInterface
	    , public UpdateMessage::SenderType
	{
	    HostInterface()
		: QuerierInterface()
		, UpdateMessage::SenderType() {}
	    
	    virtual ~HostInterface() {}

	    virtual void
	    Sent(UpdateMessage* const msg) {}
	};
    }
}

#endif // PROTOCOLS__DNS__ACTORS_HXX__
