#ifndef NETWORK__NODE_HXX__
#define NETWORK__NODE_HXX__

#include "Core/Numerical/DataTypes.h++"

namespace Network
{
    struct IMessage;
    template<typename _Node> struct Link;

    template <typename _Protocol>
    struct INode
    {
	typedef _Protocol ProtocolType;
	virtual void Send(IMessage& msg);
	virtual void Sent(Message<_Protocol>& msg);
	virtual void Received(Message<_Protocol>& msg);
    };

    template<typename _Protocol, typename _OutgoingMessageHandler, typename _IncomingMessageHandler>
    struct Node : public INode<_Protocol>
    {
	typedef _OutgoingMessageHandler OutgoingMessageHandlerType;
	typedef _IncomingMessageHandler IncomingMessageHandlerType;
	OutgoingMessageHandlerType OutgoingMessageHandler;
	IncomingMessageHandlerType IncomingMessageHandler;
	void Send(IMessage& msg)
	{
	    this->OutgoingMessageHandler.Enqueue(msg);
	}
	
	void Sent(Message<_Protocol>& msg)
	{
	    this->OutgoingMessageHandler.Sent(msg);
	}
	void Received(Message<_Protocol>& msg)
	{
	    this->IncomingMessageHandler.Received(msg);
	}
    };

    template<typename _Protocol>
    struct Link
    {
	typedef _Node NodeType;
	NodeType* Nodes;
	void Transmit(Message<_Protocol>& msg);
    };
}

#endif // NETWORK__NODE_HXX__
