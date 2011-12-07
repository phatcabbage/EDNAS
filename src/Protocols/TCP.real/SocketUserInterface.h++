#ifndef NODES__TCP__SOCKETUSERINTERFACE_HXX__
#define NODES__TCP__SOCKETUSERINTERFACE_HXX__

#include "SEDNAS.h++"

#include "Core/Signaling/SingleEvent.h++"

#include "Protocols/TCP.h++"

namespace Nodes
{
    struct SocketUserInterface
    {
	struct Message;
	struct Event;

	SingleEvent<Message&>
	OnReceive;

	SingleEvent<Event&>
	OnEvent;
	
	virtual TCP::Socket
	Address() const = 0;

	virtual bool
	IsListening() const = 0;
	
	virtual void
	Listen(bool) = 0;

	virtual void
	Connect(const TCP::Socket&) = 0;

	virtual void
	Close(const TCP::Socket&) = 0;

	virtual void
	Send(const TCP::Socket&,std::shared_ptr<void>,SizeType) = 0;

	SocketUserInterface() : OnReceive(), OnEvent() {}
	virtual ~SocketUserInterface(){}
    };	

    struct SocketUserInterface::Message
    {
	TCP::Socket		Local;
	TCP::Socket		Remote;
	std::shared_ptr<void>	Payload;
	SizeType		Bytes;
	
	Message(const TCP::Socket& l,
		const TCP::Socket& r,
		std::shared_ptr<void> p,
		SizeType bytes = 0)
	    : Local(l)
	    , Remote(r)
	    , Payload(p)
	    , Bytes(bytes)
	{}

	Message() = default;
    };

    struct SocketUserInterface::Event
    {
	enum EventType
	{
	    Connect,
	    Disconnect
	};
	
	TCP::Socket	Local;
	TCP::Socket	Remote;
	EventType Type;
    };
}
    
#endif // NODES__TCP__SOCKETUSERINTERFACE_HXX__
