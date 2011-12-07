#ifndef NODES__TCPSOCKET_HXX__
#define NODES__TCPSOCKET_HXX__

#include <iosfwd>
#include "TCPNode.h++"

#include "SocketUserInterface.h++"

namespace Nodes
{
    using Protocols::IPv4;
    using Protocols::TCP;

    struct TCPConnection;
    
    struct TCPSocket : public SocketUserInterface
    {
	TCP::Socket
	Address() const
	{ return this->Address_; }

	bool
	IsListening() const
	{ return this->IsListening_; }

	void
	Listen(bool l)
	{ this->IsListening_ = l; }

	bool
	Connect(const TCP::Socket& socket);

	void
	Close(const TCP::Socket& socket);
	
	void
	Received(const TCP::Socket& sender,
		 TCP::PacketPtr& pkt);

	void
	Send(const TCP::Socket& recv, std::shared_ptr<void> payload, SizeType bytes);


	TCPSocket(TCPNode& node,
		  TCP::Port port = 0,
		  bool listening = false)
	    : SocketUserInterface()
	    , Node_(node)
	    , Address_()
	    , Listening_(listening)
	    , Connections_()
	{
	    auto ip = node.Address();
	    Address_ = { ip, port };
	    node.Bind(*this, port);
	}

    protected:
	TCPNode&
	Node_;

	TCP::Socket
	Address_;

	bool
	Listening_;

	std::map<TCP::Socket, TCPConnection>
	Connections_;

	void
	Send(TCP::PacketPtr& pkt, const TCP::Socket& dest);
	
	friend class TCPConnection;
    };
}

    

std::ostream&
operator<<(std::ostream&,const Nodes::TCPSocket&);

#endif // NODES__TCPSOCKET_HXX__
