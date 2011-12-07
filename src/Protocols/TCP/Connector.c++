#include "Logging/OStreamLogger.h++"
#include "Connector.h++"
#include "Connection.h++"


namespace TCP
{
    static
    Logging::OStreamLogger
    Log("<Connector>");

    Connector::
    Connector(SenderType *s,
	      Port address,
	      bool listen)
	: Sender_(NULL)
	, OnConnection_()
	, Address_(Protocol::INVALID_ADDRESS)
	, Connections_()
	, Listen_(listen)
    {
	Sender(s);
	Address(address);
    }

    void
    Connector::
    Sender(SenderType* s)
    {
	Port p = Address_;
	if (Sender_)
	    Address(Protocol::INVALID_ADDRESS);
	Sender_ = s;
	if (Sender_)
	    Address(p);
    }		

    void
    Connector::
    Listening(bool l)
    {
	if (l != this->Listen_)
	{
	    this->Listen_ = l;
	    if (l) Log << *this << " now listening." << Logging::endl;
	    else Log << *this << " no longer listening." << Logging::endl;
	}
    }

    void
    Connector::
    Address(Port p)
    {
	Port oldPort = Address_;
	Address_ = p;
	if (Sender_)
	{
	    if (oldPort != Protocol::DEFAULT_ADDRESS &&
		oldPort != Protocol::INVALID_ADDRESS)
		(*Sender_)[oldPort] -= *this;
	    if (Address_ != Protocol::DEFAULT_ADDRESS &&
		Address_ != Protocol::INVALID_ADDRESS)
		(*Sender_)[Address_] += *this;
	}
    }

    Socket
    Connector::
    LocalSocket() const
    {
	if (Sender_)
	    return Socket(Sender_->IPAddress(), this->Address_);
	else
	    return Socket(IPv4::Protocol::INVALID_ADDRESS, this->Address_);
    }

    void
    Connector::
    Connect(Socket s)
    {
	auto connit = Connections_.find(s);
	if (connit != Connections_.end())
	{
	    
	    Log << *this << "Tried to create a connection on an "
		<< "already open socket." << Logging::endl;
	    return;
	}
	
	// ConnectionPtr conn = std::make_shared<Connection>
	//     (*this, s);
	ConnectionPtr conn = std::make_shared<Connection>
	    (*this, s);

	Connections_[s] = conn.get();
	conn->Connect();
	this->OnConnection_(conn);
    }

    void
    Connector::
    Received(Network::PacketListPtr& pktList)
    {
	Log << "Connector at " << Address_ << " received packet."
	    << Logging::endl;
	auto senderIP = (*pktList)[IPv4::Protocol::Layer]->Sender;
	auto senderPort = (*pktList)[TCP::Protocol::Layer]->Sender;
	Socket s(senderIP, senderPort);

	auto connit = Connections_.find(s);
	if (connit != Connections_.end())
	{
	    Log << *this << "Found receiver. Forwarding." << Logging::endl;
	    (*connit->second)(pktList);
	}
	else
	{
	    Log << *this << "Receiver not found. ";
	    if (Listen_)
	    {
		Log << "Creating new connection." << Logging::endl;
//		ConnectionPtr conn = std::make_shared<Connection>(*this, s);
		ConnectionPtr conn = 
		    std::make_shared<Connection>(*this, s);
		Connections_[s] = conn.get();
		(*conn)(pktList);
		this->OnConnection_(conn);
	    }
	    else
		Log << Logging::endl;
	}
    }

    void
    Connector::
    Send(Network::PacketListPtr& pktList, IPv4::Protocol::AddressType ip)
    {
	Log << *this << " sending packet."
	    << Logging::endl;
	auto& pkt = (*pktList)[Protocol::Layer];
	pkt->Sender = this->Address_;

	if (this->Sender_)
	    this->Sender_->Send(pktList, ip);
    }

    void
    Connector::
    Closed(Socket s)
    {
	Log << *this << " closed " << s << Logging::endl;
	auto it = Connections_.find(s);
	if (it != Connections_.end())
	    Connections_.erase(it);
    }
}

std::ostream&
operator<<(std::ostream& o, const TCP::Connector& c)
{ return o << "[Socket @ " << c.LocalSocket() << "]"; }
    
