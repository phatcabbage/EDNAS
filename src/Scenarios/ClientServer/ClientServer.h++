#ifndef SCENARIOS__CLIENTSERVER_HXX__
#define SCENARIOS__CLIENTSERVER_HXX__

#include "SEDNAS.h++"
#include "Logging/OStreamLogger.h++"
#include "Protocols/Ethernet.h++"
#include "Protocols/IPv4.h++"
#include "Protocols/TCP.h++"

namespace ClientServer
{
    struct NodeStack;
    struct Datum;
    struct Client;
    struct Server;

    typedef TCP::Socket Socket;

    typedef std::shared_ptr<TCP::Connector>  ConnectorPtr;
    typedef std::shared_ptr<TCP::Connection> ConnectionPtr;
    typedef IPv4::Protocol::AddressType IPAddress;

    typedef std::shared_ptr<NodeStack> NodeStackPtr;
    typedef std::shared_ptr<Datum> DatumPtr;
    typedef std::shared_ptr<Client> ClientPtr;
    typedef std::shared_ptr<Server> ServerPtr;


    struct NodeStack : public EventHandler<ConnectionPtr>
    {
	/* Typedefs */
	typedef std::map<int, ConnectorPtr> PortContainer;
	typedef std::list<ConnectionPtr> SocketContainer;
	typedef std::map<int, SocketContainer> 
	PortSocketContainer;

	/* Members */
	Ethernet::Node	E;
	IPv4::Node	I;
	TCP::Node	T;
	
	PortContainer
	Ports;
	
	PortSocketContainer
	Sockets;
	
	NodeStack(IPAddress ip, IPAddress gw);
	virtual ~NodeStack(){}
	
	ConnectorPtr
	GetPort(int port);

	void
	Listen(int port);
	
	void
	Connect(IPAddress destIP,int destPort)
	{ return Connect(destIP, destPort, destPort); }

	void
	Connect(IPAddress destIP,int destPort,int localPort);

void
	Connect(NodeStack& n, int destPort, int localPort);

	void
	operator()(ConnectionPtr conn)
	{ ConnectionCreated(conn); }
	
	virtual void
	ConnectionCreated(ConnectionPtr& conn); 
    };

    struct Datum : public Network::IData
    {
	const int ID;
	const SizeType Length;
	const Socket Local, Remote;
	const TimeType CreatedTime;
	TimeType SentTime, ReceivedTime;
	
	void 
	Sent();

	void 
	Received();

	Datum(SizeType length, Socket local, Socket remote);
	virtual ~Datum();

	virtual SizeType
	Size() const
	{ return Length; }
	
    protected:
	static int G_ID;

	void LogInfo();
	void Log(const std::string& s);
    };

    struct Client : public NodeStack, 
		    public EventHandler<TCP::Connection::Message&>
    {
	const int ID;
	Client(IPAddress ip, IPAddress gw);
	
	void
	operator()(TCP::Connection::Message& m)
	{ Received(m); }

     protected:
	static int G_ID;

	virtual void
	ConnectionCreated(ConnectionPtr& conn);
	
	virtual void
	Received(TCP::Connection::Message& m);
    };
    
    struct Server : public Client
    {
	const SizeType MsgSize;
	Server(IPAddress ip, IPAddress gw, SizeType msgSize);
	
	void
	SendMessage(ConnectionPtr& conn);

	virtual void
	ConnectionCreated(ConnectionPtr& conn);
    };
    
    struct Subnet
    {
	enum {CLIENT, SERVER };

	const IPAddress Net;
	IPv4::IPRouter Router;
	Ethernet::Bridge ClientBridge;
	Ethernet::Bridge ServerBridge;
	std::list<ClientPtr> Clients;
	std::list<ServerPtr> Servers;
	SizeType ServerMsgSize;

	Subnet(int class_b,
	       SizeType serverMsg,
	       SizeType clientBW = 128 * Bandwidth::KILOBIT,
	       SizeType serverBW = 128 * Bandwidth::KILOBIT,
	       TimeType clientDelay = 30 * Time::MILLISECOND,
	       TimeType serverDelay = 30 * Time::MILLISECOND);

	ClientPtr
	AddClient(SizeType bw = 128 * Bandwidth::KILOBIT,
		  TimeType delay = 30 * Time::MILLISECOND);
	
	ServerPtr
	AddServer(SizeType bw = 128 * Bandwidth::KILOBIT,
		  TimeType delay = 30 * Time::MILLISECOND);
    };
}
#endif // SCENARIOS__CLIENTSERVER_HXX__
