#include "TCPConnectionHandler.h++"

namespace Nodes
{

    // < TCP::Closed >
    template<>
    void
    ConnectionStateHandler<TCP::Closed>::Init()
    {
	Conn.TCB.reset();
    }

    template<>
    void
    ConnectionStateHandler<TCP::Closed>::
    Received(TCP::PacketPtr& pkt)
    {
	TCPLog::Log << "Closed [Socket " << Conn.Source() << "] received packet."
	       << Logging::endl;
    }
    // </ TCP::Closed >

    // < TCP::Listening >
    template<>
    void
    ConnectionStateHandler<TCP::Listening>::Init()
    {
	Conn.TCB.reset(new TCPConnection::ControlBlock());

	auto tcb = Conn.TCB;
	tcb->clear();
	tcb->LocalSeq = random();
	tcb->AckedSent = tcb->LocalSeq;
    }

    template<>
    void
    ConnectionStateHandler<TCP::Listening>::
    Received(TCP::PacketPtr& pkt)
    {
	if (pkt->Flags != TCP::SYN)
	{
	    TCPLog::Log << "|(Listening)| " << "Erroneous packet on listening socket." << Logging::endl;
	    TCPLog::Log << "|(Listening)| " << "\tFlags = " << static_cast<int>(pkt->Flags) << Logging::endl;
	    Conn.TCB.Destination = { 0, 0 };
	}
	else
	{
	    TCPLog::Log << "|(Listening)| " << "Listening socket received connection request." << Logging::endl;

	    Conn.TCB.AckedRcvd	= pkt->SEQ;
	    Conn.TCB.RemoteSeq	= pkt->SEQ + pkt->DataBytes();

	    Conn.ChangeState<TCP::SynReceived>();
	}
    }
    // </ TCP::Listening >

    // < TCP::SynReceived> 
    template<>
    void
    ConnectionStateHandler<TCP::SynReceived>::Init()
    {
	TCPLog::Log << "|(SynReceived)| " << this->Conn.Node_ << " SYN received, synchronizing..." << Logging::endl;
	auto pkt = std::make_shared<TCP::Packet>();
	TCPLog::Log << "|(SynReceived)| " << this->Conn.Node_ << " Packet flags: " << static_cast<int>(pkt->Flags) << Logging::endl;
	pkt->Flags += TCP::SYN; // ACK is automatically done by Connection
	TCPLog::Log << "|(SynReceived)| " << this->Conn.Node_ << " Packet flags: " << static_cast<int>(pkt->Flags) << Logging::endl;
	Conn.Send(pkt);
    }
    
    template<>
    void
    ConnectionStateHandler<TCP::SynReceived>::
    Received(TCP::PacketPtr& pkt)
    {
	if (pkt->ACK != Conn.TCB->LocalSeq)
	{
	    TCPLog::Log << "|(SynReceived)| " << "Connection failed due to mismatched ACKs." << Logging::endl
			<< "|(SynReceived)| " <<  "\tDesired:  " << (Conn.TCB.LocalSeq + 1) << Logging::endl
			<< "|(SynReceived)| " <<  "\tReceived: " << pkt->ACK << Logging::endl;
	    return; // incorrect ACK
	}
	
	Conn.TCB->AckedSent = pkt->ACK;
	TCPLog::Log << "|(SynReceived)| " << "[Socket " << Conn.Source() << "] successfully connected to "
		    << "[Socket " << Conn.Destination() << "]!" << Logging::endl;
	
	Conn.ChangeState<TCP::Established>();
    }
    // </ TCP::SynReceived>

    // < TCP::SynSent>
    template<>
    void
    ConnectionStateHandler<TCP::SynSent>::Init()
    {
	TCPLog::Log << "|(SynSent)| " << "[" << Conn.Node_.Address() << "] Transitioning to SynSent state." << Logging::endl;
	auto& tcb	= Conn.TCB;

	tcb.LocalSeq		= random() / Size::BYTE;
	tcb.AckedSent		= tcb.LocalSeq;
	tcb.AckedRcvd		= tcb.RemoteSeq;

	auto pkt = std::make_shared<TCP::Packet>();
	pkt->Flags = TCP::SYN;
	Conn.Send(pkt);
    }

    template<>
    void
    ConnectionStateHandler<TCP::SynSent>::
    Received(TCP::PacketPtr& pkt)
    {
	if (!(pkt->Flags & (TCP::SYN + TCP::ACK)))
	{
	    TCPLog::Log << "|(SynSent)| " << "Connection rejected due to lack of SYN & ACK combination. " << Logging::endl;
	    return;
	}
	
	if (pkt->ACK != Conn.TCB.LocalSeq)
	{
	    TCPLog::Log << "|(SynSent)| " << "Connection rejected due to ACK mismatches." << Logging::endl;
	    return;
	}

	Conn.TCB.RemoteSeq	= pkt->SEQ + pkt->DataBytes();
	Conn.TCB.AckedRcvd     += pkt->SEQ;
	
	TCPLog::Log << "|(SynSent)| " << this->Conn.Node_ << " Successfully connected to " << Conn.TCB.Destination << Logging::endl;
	
	Conn.ChangeState<TCP::Established>();
    }

    template<>
    ConnectionStateHandler<TCP::SynSent>::
    ~ConnectionStateHandler()
    {
	auto pkt = std::make_shared<TCP::Packet>();
	Conn.Send(pkt); // ACK packet, constructed by TCPSocket::Connection
    }

    // </ TCP::SynSent>


    // < TCP::Established >
    template<>
    void
    ConnectionStateHandler<TCP::Established>::Init()
    {
	TCPLog::Log << this->Conn.Node_ << " WOO HOO" << Logging::endl;
	this->Conn.Node_.OnConnected(Conn.TCB.Destination);
	
	if (!Conn.Outqueue_.empty)
	{
	    MessageEntry& m = Conn.Outqueue_.front();
	    auto pkt = std::make_shared<TCP::Packet>();
	    pkt->Type = TCP::FAKE;
	    pkt->Payload = m.Payload;
	    pkt->Size = m.ByteSize * Size::BYTE;
	    
	    Conn.Outqueue_.pop_front();
	    Conn.Send(pkt);
	}
    }

    template<>
    void
    ConnectionStateHandler<TCP::Established>::
    Received(TCP::PacketPtr& pkt)
    {
	auto bytesize = pkt->ByteSize();
	Conn.TBD->RemoteSeq += bytesize;
	Conn.Received(pkt->Payload, bytesize);
	if (pkt->Flags & TCP::ACK)
	    Conn.TBD->AckedSent = pkt->ACK;

	if (!Conn.Outqueue_.empty)
	{
	    MessageEntry& m = Conn.Outqueue_.front();
	    auto pkt = std::make_shared<TCP::Packet>();
	    pkt->Type = TCP::FAKE;
	    pkt->Payload = m.Payload;
	    pkt->Size = m.ByteSize * Size::BYTE;
	    
	    Conn.Outqueue_.pop_front();
	    Conn.Send(pkt);
	}
	else
	{
	    if (bytesize == 0) return;
	    auto pkt = std::make_shared<TCP::Packet>();
	    Conn.Send(pkt); // just an ACK packet
	}
    }	
    // </ TCP::Established >
    

//     template<TCP::ConnectionState S>
//     void
//     ConnectionStateHandler<S>::Init()
//     { TCPLog::Log << "Warning: Generic ConnectionHandler init called." << Logging::endl; }

//     template<TCP::ConnectionState S>
//     void
//     ConnectionStateHandler<S>::Received(TCP::PacketPtr& pkt)
//     { TCPLog::Log << "Warning: Generic ConnectionHandler receiver called." << Logging::endl; }
    
    
//     template<TCP::ConnectionState S>
//     ConnectionStateHandler<S>::
//     ~ConnectionStateHandler()
//     { TCPLog::Log << "Warning: Generic ConnectionHandler destructor called." << Logging::endl; }
}
