#include <iomanip> // for pretty-printing time
#include <fstream> // for the log file
#include <sstream> // of pretty-printing time
#include "ClientServer.h++"
#include "Simulation/Timer.h++"
#include "Protocols/IPv4/IPRouterPort.h++"

namespace ClientServer
{
    Logging::FileLogger
    DataLog("Data");

    int
    Datum::
    G_ID(0);

    int
    Client::
    G_ID(0);

    NodeStack::
    NodeStack(IPAddress ip, IPAddress gw)
	: E()
	, I(&E, ip, IPv4::Protocol::CLASS_C, gw)
	, T(&I)
	, Ports()
	, Sockets()
    {}

    ConnectorPtr
    NodeStack::
    GetPort(int port)
    {
	ConnectorPtr& conn = Ports[port];
	if(!conn)
	{
	    conn = std::make_shared<TCP::Connector>(&T, port);
	    conn->OnConnection() += *this;
	}
	return conn;
    }
	  
    void
    NodeStack::
    Listen(int port)
    {
	ConnectorPtr conn = GetPort(port);
	conn->Listening(true);
    }
   
    void
    NodeStack::
    Connect(IPAddress destIP, int destPort, int localPort)
    {
	ConnectorPtr contor = GetPort(localPort);
	SocketContainer& sockets = Sockets[localPort];
	ConnectionPtr conn;
	Socket remote = { destIP, destPort };
	for(SocketContainer::iterator it = sockets.begin(),
		end = sockets.end(); it != end; ++it)
	{
	    if ((*it)->RemoteSocket() == remote)
	    {
		conn = *it;
		break;
	    }
	}
	
	if(!conn)
	    contor->Connect(remote);
    }

    void
    NodeStack::
    Connect(NodeStack& n, int destPort, int localPort)
    {
	IPAddress ip = n.I.Address();
	Connect(ip, destPort, localPort);
    }

    void
    NodeStack::
    ConnectionCreated(ConnectionPtr& conn)
    {
	Sockets[conn->LocalSocket().Port].push_back(conn);
    }
    
    Datum::
    Datum(SizeType length, Socket local, Socket remote)
	: ID(++G_ID)
	, Length(length)
	, Local(local)
	, Remote(remote)
	, CreatedTime(Timer::Main.Time())
	, SentTime(-1)
	, ReceivedTime(-1)
    {
	LogInfo();
	Log("CREATED");
    }

    Datum::
    ~Datum()
    { Log("DESTROYED"); }

    void
    Datum::
    Sent()
    {
	SentTime = Timer::Main.Time();
	Log("SENT");
    }

    void
    Datum::
    Received()
    {
	ReceivedTime = Timer::Main.Time();
	Log("RECEIVED");
    }
    
    void
    Datum::
    LogInfo()
    {
	DataLog << TimeToString(Timer::Main.Time()) << "\t" 
		<< "INFO" << "\t" 
		<< ID     << "\t" 
		<< Length << "\t" 
		<< Local  << "\t" 
		<< Remote
		<< Logging::endl;
    }

    void
    Datum::
    Log(const std::string& s)
    {
	DataLog << TimeToString(Timer::Main.Time()) << "\t" 
		<< s  << "\t" 
		<< ID << "\t" 
		<< Logging::endl;
    }
    
    Client::
    Client(IPAddress ip, IPAddress gw)
	: NodeStack(ip, gw)
	, ID(++G_ID)
    {}

    void
    Client::
    ConnectionCreated(ConnectionPtr& conn)
    {
	NodeStack::ConnectionCreated(conn);
	conn->OnReceive() += *this;
    }

    void
    Client::
    Received(TCP::Connection::Message& m)
    {
	DatumPtr datum 
	    = std::static_pointer_cast<Datum>(m.Data);

	datum->Received();
    }

    Server::
    Server(IPAddress ip, IPAddress gw, SizeType msgSize)
	: Client(ip, gw)
	, MsgSize(msgSize)
    {}	  

    void
    Server::
    SendMessage(ConnectionPtr& conn)
    {
	DatumPtr datum = 
	    std::make_shared<Datum>
	    (MsgSize, conn->LocalSocket(), 
	     conn->RemoteSocket());
	conn->Send(datum);
	datum->Sent();
    }

    void
    Server::
    ConnectionCreated(ConnectionPtr& conn)
    {
	Client::ConnectionCreated(conn);
	SendMessage(conn);
    }

    Subnet::
    Subnet(int class_b,
	   SizeType serverMsg,
	   SizeType clientBW,
	   SizeType serverBW,
	   TimeType clientDelay,
	   TimeType serverDelay)
	: Net(class_b << 16)
	, Router()
	, ClientBridge()
	, ServerBridge()
	, Clients()
	, Servers()
	, ServerMsgSize(serverMsg)
    {
	IPAddress clientgw = Net + (CLIENT << 8) + 1;
	IPAddress servergw = Net + (SERVER << 8) + 1;
	
	int clientIf = 
	    Router.AddInterface(clientgw);
	Ethernet::NodeBase& clientNode = 
	    *(Router[clientIf].Sender());

	int serverIf = 
	    Router.AddInterface(servergw);
	Ethernet::NodeBase& serverNode =
	    *(Router[serverIf].Sender());

	ClientBridge.SetDefaultLinkBandwidth(100 * Bandwidth::MEGABIT);
	ClientBridge.SetDefaultLinkDelay(1 * Time::MILLISECOND);
	ServerBridge.SetDefaultLinkBandwidth(100 * Bandwidth::MEGABIT);
	ServerBridge.SetDefaultLinkDelay(1 * Time::MILLISECOND);
	
	ClientBridge += clientNode;
	ServerBridge += serverNode;
    }
    
    ClientPtr
    Subnet::
    AddClient(SizeType bw, TimeType delay)
    {
	IPAddress ip = Net + (CLIENT << 8) + Clients.size() + 2;
	IPAddress gw = Net + (CLIENT << 8) + 1;

	ClientBridge.SetDefaultLinkBandwidth(bw);
	ClientBridge.SetDefaultLinkDelay(delay);

	ClientPtr c = std::make_shared<Client>(ip, gw);

	ClientBridge += c->E;
	Clients.push_back(c);
	return c;
    }

    ServerPtr
    Subnet::
    AddServer(SizeType bw, TimeType delay)
    {
	IPAddress ip = Net + (SERVER << 8) + Servers.size() + 2;
	IPAddress gw = Net + (SERVER << 8) + 1;
	
	ServerBridge.SetDefaultLinkBandwidth(bw);
	ServerBridge.SetDefaultLinkDelay(delay);

	ServerPtr s = std::make_shared<Server>
	    (ip, gw, ServerMsgSize);

	ServerBridge += s->E;
	Servers.push_back(s);
	return s;
    }
}
