#include "TCPConnectionHandler.c++"
#include "TCPConnection.c++"

namespace Nodes
{
    TCP::ConnectionState
    TCPConnection::
    State() const
    { return Handler_->State(); }
	
    void
    TCPConnection::
    Listen()
    {
	this->ChangeState<TCP::Listening>();
    }

    void
    TCPConnection::
    Connect()
    {
	this->ChangeState<TCP::SynSent>();
    }	
    
    void
    TCPConnection::
    Close()
    {
	this->ChangeState<TCP::Closed>();
    }

    TCPConnection::
    Connection(TCPSocket& node, const TCP::Socket& dest)
	: Node_(node)
	, Destination_(dest)
	, Handler_(NULL)
	, TCB(NULL)
    {
	if (node->IsListening())
	    this->Handler_.reset(new ConnectionStateHandler<TCP::Listening>(*this));
	else
	    this->Handler_.reset(new ConnectionStateHandler<TCP::Closed>(*this));
    }

    void
    TCPConnection::
    Received(TCP::PacketPtr& pkt)
    {
	this->Handler_->Received(pkt); 
    }

    template<TCP::ConnectionState S>
    void
    TCPConnection::
    ChangeState()
    {
	if (this->State() == S) return;
	this->Handler_ = new ConnectionStateHandler<S>(*this);
    }

    void
    TCPConnection::
    Send(TCP::PacketPtr& pkt)
    {
	SizeType size = pkt->DataBytes();
	pkt->SEQ = TCB->LocalSeq;
	TCB->LocalSeq += size;

	/* have we acknowledged the most recent data? */
	if (TCB->RemoteSeq > TCB->AckedRcvd)
	{
	    TCB->AckedRcvd = TCB->RemoteSeq;
	    pkt->Flags += TCP::ACK;
	    pkt->ACK    = TCB->AckedRcvd;
	}

	TCPLog::Log << this->Node_ << " Sending packet with flags: " << static_cast<int>(pkt->Flags) << Logging::endl;
	Node_.Send(pkt, TCB->Destination);
    }
}
