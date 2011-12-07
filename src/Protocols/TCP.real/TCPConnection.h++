#ifndef NODES__TCPCONNECTION_HXX__
#define NODES__TCPCONNECTION_HXX__

#include "SEDNAS.h++"


namespace Nodes
{
    struct TCPSocket;

    struct TCPConnection
    {
	struct Handler;
	struct MessageEntry;
	struct ControlBlock;

	TCP::ConnectionState
	State() const;

	TCPSocket&
	Node() const
	{ return Node_; }

	TCP::Socket
	Source() const
	{ return this->Node_.Address(); }

	TCP::Socket
	Destination() const
	{ return this->TCB.Destination; }
	
	void 
	Listen();

	void 
	Connect();

	void 
	Close();

	Connection(TCPSocket& node, const TCP::Socket& dest);

	void 
	Received(TCP::PacketPtr& pkt);

    protected:
	TCPSocket&
	Node_;

	TCP::Socket
	Destination_;
	
	std::unique_ptr<Handler>
	Handler_;

	std::unique_ptr<ControlBlock>
	TCB;

	template<TCP::ConnectionState S> 
	void ChangeState();

	void Send(TCP::PacketPtr& pkt);

	void Received(std::shared_ptr<void> payload, SizeType bytesize);

	template <TCP::ConnectionState S> friend struct ConnectionStateHandler;
    };
    
    struct TCPConnection::MessageEntry
    {
	std::shared_ptr<void> Payload;
	SizeType              ByteSize;
    };

    struct TCPConnection::ControlBlock
    {
	TCP::SequenceNumber		LocalSeq;
	TCP::SequenceNumber		AckedSent;
	
	TCP::SequenceNumber		RemoteSeq;
	TCP::SequenceNumber		AckedRcvd;

	std::list<MessageEntry>		OutQueue_;
	
	std::list<TCP::PacketPtr>	InQueue_;
    };
}

#endif // NODES__TCPCONNECTION_HXX__
