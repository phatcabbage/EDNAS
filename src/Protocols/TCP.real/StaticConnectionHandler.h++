#include "TCPConnection.h++"

namespace Nodes
{
    struct TCPConnection::Handler
    {
	/* Active open (ie. Connect(...) */
	static void Open	(TCPSocket& n, TCPConnection& conn, const TCP::Socket& remote);

	/* Passive open (ie. Listen() */
	static void Open	(TCPSocket& n, TCPConnection& conn);

	/* Close */
	static void Close	(TCPSocket& n, TCPConnection& conn);
	
	static void Received	(TCPSocket& n, TCPConnection& conn, TCP::PacketPtr& pkt);
	
	static void Send	(TCPSocket& n, TCPConnection& conn, TCP::PacketPtr& pkt);

	static TCP::ConnectionState  Status (TCPSocket& n, TCPConnection& conn);
    };

    template<TCP::ConnectionState S>
    struct ConnectionStateHandler : public TCPConnection::Handler
    {
	TCP::ConnectionState
	State() const 
	{ return S; }

	void 
	Init();
	
	virtual void 
	Received(TCP::PacketPtr& pkt);

 	ConnectionStateHandler(TCPConnection& conn) : TCPConnection::Handler(conn)
	{ this->Init(); }
	
 	virtual ~ConnectionStateHandler();

    };

    template class ConnectionStateHandler<TCP::Closed>;
    template class ConnectionStateHandler<TCP::Listening>;
    template class ConnectionStateHandler<TCP::SynSent>;
    template class ConnectionStateHandler<TCP::SynReceived>;
    template class ConnectionStateHandler<TCP::Established>;
    template class ConnectionStateHandler<TCP::FinWait1>;
    template class ConnectionStateHandler<TCP::FinWait2>;
    template class ConnectionStateHandler<TCP::CloseWait>;
    template class ConnectionStateHandler<TCP::Closing>;
    template class ConnectionStateHandler<TCP::LastAck>;

    template<TCP::ConnectionState S>
    TCPConnection::Handler*
    TCPConnection::Handler::
    StateHandler<S>(TCPConnection& conn)
    {	return new ConnectionStateHandler<S>(conn);    }
}
