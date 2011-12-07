#ifndef PROTOCOLS__PING__INTERFACES_HXX__
#define PROTOCOLS__PING__INTERFACES_HXX__

#include <iostream>

//#include "Foundation/Network/Node.h++"
#include "./Messages.h++"
#include "SEDNAS.h++"

namespace Protocols
{
    namespace Ping
    {
	class NodeInterface : public PingMessage::SenderType
			    , public PingMessage::ReceiverType
			    , public PongMessage::SenderType
			    , public PongMessage::ReceiverType
	{
	public:
	    NodeInterface()
		: PingMessage::SenderType()
		, PingMessage::ReceiverType()
		, PongMessage::SenderType()
		, PongMessage::ReceiverType() {}

	    virtual ~NodeInterface() {}

	    virtual void
	    Sent(PingMessage* const msg)
	    {
		std::cout << "[APP] " << __PRETTY_FUNCTION__ << std::endl;
	    }

	    virtual void
	    Received(PingMessage* const msg)
	    {
		std::cout << "[APP] " << __PRETTY_FUNCTION__ << std::endl;
	    }

	    virtual void
	    Sent(PongMessage* const msg)
	    {
		std::cout << "[APP] " << __PRETTY_FUNCTION__ << std::endl;
	    }

	    virtual void
	    Received(PongMessage* const msg)
	    {
		std::cout << "[APP] " << __PRETTY_FUNCTION__ << std::endl;
	    }
	};
    }
}
#endif // PROTOCOLS__PING__INTERFACES_HXX__
