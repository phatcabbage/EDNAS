#include "Simulation/Timer.h++"
#include "TFTPServer.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Nodes::TFTPServer>::
    Logger("TFTPServer");
}

namespace Nodes
{
    const static UDP::Port TFTPServerPort = TFTP::ServerPort;

    void
    TFTPServer::
    ReceivedReadRequest(IPv4::AddressType ip,
			UDP::AddressType port,
			const std::string& filename)
    {
	Logger << "Read request from " << ip
	       << ":" << port << " for \""
	       << filename << "\"" << Logging::endl;

	if(Files_.count(filename) < 1)
	{
	    this->SendError(TFTP::FileNotFound, ip, port);
	    return;
	}
	
	if (!(this->Outgoing_.count(port)))
	{
	    Transfer& t = this->Outgoing_[port];
	    t = { Files_[filename], ip, port, 0 };
	
	    this->SendData(t);
	}
    }

    void
    TFTPServer::
    ReceivedWriteRequest(IPv4::AddressType ip,
			 UDP::AddressType port,
			 const std::string& filename)
    {
	Logger << "Write request from " << ip
	       << ":" << port << " for \""
	       << filename << "\"" << Logging::endl;

	if (this->ReadOnly_)
	{
	    this->SendError(TFTP::AccessViolation, ip, port);
	    return;
	}
	
	if (!this->Incoming_.count(port))
	{
	    Transfer& t = this->Incoming_[port];
	    TFTP::File f = { filename, 0 };
	    t = { f, ip, port, 0 };
	
	    this->SendAcknowledgment(t);
	}
    }

    void
    TFTPServer::
    ReceivedAcknowledgment(IPv4::AddressType ip,
			   UDP::AddressType port,
			   TFTP::PacketType::BlockID block)
    {
	if (!Outgoing_.count(port))
	{
	    this->SendError(TFTP::UnknownID, ip, port);
	    return;
	}

	Transfer& t = Outgoing_[port];
	if(block > t.LastACK)
	{
	    t.Timeouts = 0;
	    t.LastACK = block;
	    this->SendData(t);
	}
    }

    void
    TFTPServer::
    ReceivedData(IPv4::AddressType ip,
		 UDP::AddressType port,
		 TFTP::PacketType::BlockID block,
		 SizeType size)
    {
	if(!Incoming_.count(port))
	{
	    this->SendError(TFTP::UnknownID, ip, port);
	    return;
	}

	Transfer& t = Incoming_[port];
	if (block == (t.LastACK + 1))
	{
	    t.Timeouts = 0;
	    ++(t.LastACK);
	    this->SendAcknowledgment(t);
	    
	    if (size != TFTP::MAX_DATA + 32)
	    {
		t.File.Size = (TFTP::MAX_DATA * (t.LastACK - 1)) + (size - 32);
		this->IncomingCompleted(t);
	    }
	}
    }

    void
    TFTPServer::
    ReceivedError(IPv4::AddressType ip,
		  UDP::AddressType port,
		  TFTP::ErrorCode error)
    {
	Logger << "Error." << Logging::endl;
    }

    void
    TFTPServer::
    SendError(TFTP::ErrorCode error,
	      IPv4::AddressType ip,
	      UDP::AddressType port)
    {
	TFTP::PacketPtr errpkt(new TFTP::Message<TFTP::Error>
			       (error));
	
	UDP::PacketPtr udppkt = std::make_shared<UDP::PacketType>
	    (UDP::TFTP, errpkt, errpkt->Size(),
	     TFTPServerPort,
	     port);
	
	this->Node_.Send(udppkt, ip);
    }
    
    void
    TFTPServer::
    SendData(Transfer& t)
    {
	SizeType bitsleft = t.File.Size - (t.LastACK * TFTP::MAX_DATA);
	if (bitsleft < 0)
	{
	    OutgoingCompleted(t);
	    return;
	}
	
	SizeType bitstosend = std::min(TFTP::MAX_DATA, bitsleft);
	TFTP::PacketPtr datapkt(new TFTP::Message<TFTP::Data>
				(t.LastACK + 1, bitstosend));
	UDP::PacketPtr udppkt = std::make_shared<UDP::PacketType>
	    (UDP::TFTP, datapkt, datapkt->Size(), 
	     t.Port, TFTPServerPort);
	
	Logger << "Sending Block " << (t.LastACK + 1) << " to "
	       << t.IP << ":" << t.Port << Logging::endl;

	this->Node_.Send(udppkt, t.IP);

	auto tt = new TransferTimer(*this, t.Port, TransferTimer::Outgoing,
				    t.LastACK);
	Simulation::Timer::Main.Schedule(this->Timeout(), *tt);
    }

    void
    TFTPServer::
    SendAcknowledgment(Transfer& t)
    {
	TFTP::PacketPtr ackpkt(new TFTP::Message<TFTP::Acknowledgment>(t.LastACK));
	UDP::PacketPtr udppkt = std::make_shared<UDP::PacketType>
	    (UDP::TFTP, ackpkt, ackpkt->Size(), t.Port, TFTPServerPort);

	this->Node_.Send(udppkt, t.IP);

	auto tt = new TransferTimer(*this, t.Port, TransferTimer::Incoming,
				    t.LastACK);
	Simulation::Timer::Main.Schedule(this->Timeout(), *tt);

    }

    void
    TFTPServer::
    IncomingTimedOut(Transfer& t)
    {
	if (++t.Timeouts > TIMEOUTS)
	    this->Incoming_.erase(t.Port);
	else
	    this->SendAcknowledgment(t);
    }

    void
    TFTPServer::
    OutgoingTimedOut(Transfer& t)
    {
	if (++t.Timeouts > TIMEOUTS)
	    this->OutgoingCompleted(t);
	else
	    this->SendData(t);
    }

    void
    TFTPServer::
    IncomingCompleted(Transfer& t)
    {
	this->AddFile(t.File.Name, t.File.Size);
	this->Incoming_.erase(t.Port);
    }

    void
    TFTPServer::
    OutgoingCompleted(Transfer& t)
    {
	this->Outgoing_.erase(t.Port);
    }

    void
    TFTPServer::
    Received(UDPNode::MessageArg& msg)
    {
	Logger << "Received a message." << Logging::endl;
	if (msg.UDPPkt->Type != UDP::TFTP) return;

	auto tftppkt = std::static_pointer_cast
	    <TFTP::PacketType>(msg.UDPPkt->Payload);

	auto ip = msg.IPPkt->Sender;
	auto port = msg.UDPPkt->Sender;
	
	switch(tftppkt->Type)
	{
	case (TFTP::ReadRequest):
	{
	    std::string name = 
		(static_cast<TFTP::Message<TFTP::ReadRequest>*>(tftppkt.get()))->
		FileName;
		
	    ReceivedReadRequest(ip, port, name);
	    break;
	}
	case (TFTP::WriteRequest):
	{
	    std::string name = 
		(static_cast<TFTP::Message<TFTP::WriteRequest>*>(tftppkt.get()))->
		FileName;

	    ReceivedWriteRequest(ip, port, name);
	    break;
	}
	case (TFTP::Acknowledgment):
	{
	    TFTP::PacketType::BlockID block = 
		(static_cast<TFTP::Message<TFTP::Acknowledgment>*>(tftppkt.get()))->
		Block;

	    ReceivedAcknowledgment(ip, port, block);
	    break;
	}
	case (TFTP::Data):
	{
	    auto tpkt = static_cast<TFTP::Message<TFTP::Data>*>(tftppkt.get());
	    ReceivedData(ip, port, tpkt->Block, tpkt->Size());
	    break;
	}
	case (TFTP::Error):
	{
	    auto error = static_cast<TFTP::Message<TFTP::Error>*>
		(tftppkt.get())->Code;
	    ReceivedError(ip, port, error);
	    break;
	}
	default:
	{
	    this->SendError(TFTP::IllegalOperation, ip, port);
	    break;
	}
	}
    }
}
