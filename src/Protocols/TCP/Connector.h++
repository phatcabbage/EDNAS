#ifndef TCP__CONNECTOR_HXX__
#define TCP__CONNECTOR_HXX__

#include "SEDNAS.h++"
#include "Simulation/Event.h++"
#include "Protocol.h++"
#include "Packet.h++"
#include "Node.h++"

namespace TCP
{
    struct Connection;

    struct Connector : public EventHandler<Network::PacketListPtr&>
    {
	typedef Node SenderType;
	typedef IPv4::Protocol::AddressType IPAddress;
	typedef std::shared_ptr<Connection> ConnectionPtr;
	
	SenderType*
	Sender() const
	{ return Sender_; }
	
	Event<ConnectionPtr>&
	OnConnection()
	{ return this->OnConnection_; }
	
	Port
	Address() const
	{ return Address_; }
    
	bool
	Listening() const
	{ return Listen_; }
	
	void
	Listening(bool l);

	void
	Sender(SenderType* s);
	
        Socket
	LocalSocket() const; 

	void
	Address(Port p);
	
	void
	Connect(Socket s);

	void 
	Received(Network::PacketListPtr& pktList);

	void
	Send(Network::PacketListPtr& pktList, IPv4::Protocol::AddressType ip);

	void
	Closed(Socket s);

	Connector(SenderType* s = NULL,
		  Port address = Protocol::DEFAULT_ADDRESS,
		  bool listen = false);

	virtual void
	operator()(Network::PacketListPtr& pktList)
	{ this->Received(pktList); }

    protected:
	SenderType* Sender_;
	SingleEvent<ConnectionPtr> OnConnection_;
	Port Address_;
	std::map<Socket, Connection*> Connections_;
	bool Listen_;
	void
	Drop(Network::PacketListPtr& pkt);
    };
}

std::ostream&
operator<<(std::ostream&, const TCP::Connector&);

#endif // TCP__CONNECTOR_HXX__
