#include <cassert>
#include <cstdlib> // for random
#include <cstring> // for memset

#include "TCPConnection.h++"
#include "TCPSocket.h++"

namespace Nodes
{
    bool
    TCPSocket::
    Connect(const TCP::Socket& socket)
    {
// 	TCPLog::Log << *this <<  " Attempting connection to " << socket << Logging::endl;
// 	if (Connections_.count(socket))
// 	{
// 	    TCPLog::Log << "Attempt to create a connection to already-connected socket!" << Logging::endl;
// 	    return;
// 	}

// 	auto conn = new Connection(*this);
// 	Connections_[socket] = conn;
// 	conn->Connect(socket);
	auto& conn = Connections_[socket];
	return conn.Connect(socket);
    }

    void
    TCPSocket::
    Received(const TCP::Socket& s, TCP::PacketPtr& pkt)
    {
	TCP::Socket s = { ip, port };
	TCPLog::Log << *this << " Received TCP packet from " << s << Logging::endl;

	auto& conn = Connections_[s];
	conn.Received(pkt);
	
// 	auto it = Connections_.find(s);
// 	if (it != Connections_.end())
// 	{
// 	    TCPLog::Log << *this << " Connection found." << Logging::endl;
// 	    it->second->Received(pkt);
// 	}
// 	else if (Listening_)
// 	{
// 	    TCPLog::Log << *this << " No Connection found, but local socket is listening." << Logging::endl;
// 	    auto conn = this->AcceptIncoming(s, pkt);
// 	    if (conn)
// 		Connections_[s] = conn;
// 	}
// 	else
// 	{
// 	    TCPLog::Log << "Connection not found and socket is not listening." << Logging::endl
// 			<< "Open sockets: " << Logging::endl;
// 	    for (auto lit = Connections_.begin(), lend = Connections_.end();
// 		 lit != lend; 
// 		 ++lit)
// 		TCPLog::Log << "\t" << lit->first << Logging::endl;
// 	}
    }

    void
    TCPSocket::
    Close(const TCP::Socket& socket)
    {
// 	auto it = Connections_.find(socket);
// 	if (it != Connections_.end())
// 	{
// 	    TCPConnection* conn = *it;
// 	    Connections_.erase(it);
// 	    if (conn->State() != TCP::Closed)
// 		conn->Close();
	    
// 	    SocketUserInterface event = { this->Address_, s, Event::Disconnect };
// 	    this->OnEvent(event);
// 	}
	auto& conn = Connections_[socket];
	conn.Close();
    }

    void
    TCPSocket::
    Send(const TCP::Socket& s, std::shared_ptr<void> p, SizeType bytes)
    {
// 	auto it = Connections_.find(s);
// 	if (it != Connections_.end())
// 	{
// 	    it->second->Enqueue(m);
    }

    void
    TCPSocket::
    Send(TCP::PacketPtr& pkt, const TCP::Socket& dest)
    {
	pkt->Sender   = Address_.Port;
	pkt->Receiver = dest.Port;
	this->Node_.Send(pkt, dest.IP);
    }

//     TCPConnection*
//     TCPSocket::
//     AcceptIncoming(const TCP::Socket& s, TCP::PacketPtr& pkt)
//     {
// 	auto conn = new TCPConnection(*this);
// 	conn->Listen();
// 	conn->Destination(s);
// 	conn->Received(pkt);

// 	if (conn->Destination() != s)
// 	{
// 	    delete conn;
// 	    return NULL;
// 	}

// 	return conn;
//     }
}

std::ostream&
operator<<(std::ostream& o, const Nodes::TCPSocket& t)
{ return o << "[Socket |" << t.Address() << "|]"; }
