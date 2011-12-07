#include <algorithm>
#include <cstdlib>
#include "Simulation/Timer.h++"
#include "TFTPClient.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Nodes::TFTPClient>::
    Logger("TFTPClient");
}

namespace Nodes
{
    typedef TFTP::Message<TFTP::ReadRequest>  ReadRequestMessage;
    typedef TFTP::Message<TFTP::WriteRequest> WriteRequestMessage;

    UDP::Port
    TFTPClient::
    RandomPort()
    {
	int32_t randport = random();
	randport %= (UDP::MAX_PORT.Value - UDP::MIN_UNPRIVILEGED_PORT.Value);
	randport += UDP::MIN_UNPRIVILEGED_PORT.Value;
	return UDP::Port(randport);
    }

    TFTPClient::
    TFTPClient(UDPNode& node,
	       TimeType timeout)
	: OnTransferEvent()
	, Node_(node)
	, ReceivedAdapter_(*this, &TFTPClient::Received)
	, Busy_(false)
	, Timeout_(timeout)
	, Timeouts_(0)
	, Port_(0)
	, Server_(IPv4::DEFAULT)
	, Block_(0)
	, File_()
    {}
    
    TFTPClient::
    ~TFTPClient()
    {}

    void
    TFTPClient::
    SendFile(const std::string& filename,
	     SizeType size)
    {
	if (this->Busy_)
	    throw std::string("Busy TFTPClient asked to send file.");
	if (this->Server_ == IPv4::DEFAULT)
	    throw std::string("Invalid Server IP.");
	
	this->Busy_ = true;
	this->Timeouts_ = 0;

	do { this->Port_ = RandomPort(); } 
	while (this->Node_.Bound(this->Port_));

	this->Node_.Bind(this->ReceivedAdapter_, this->Port_);

	Logger << "TFTPClient received [Port " << this->Port_ 
	       << "] for send." << Logging::endl;
	
	this->Block_ = 0;
	this->File_ = { filename, size };
	this->SendWriteRequest();
    }

    void
    TFTPClient::
    RequestFile(const std::string& filename)
    {
	if (this->Busy_)
	    throw std::string("Busy TFTPClient asked to request file.");
	if (this->Server_ == IPv4::DEFAULT)
	    throw std::string("Invalid Server IP.");
	
	this->Busy_ = true;
	this->Timeouts_ = 0;

	do { this->Port_ = RandomPort(); }
	while (this->Node_.Bound(this->Port_));

	if (!(this->Node_.Bind(this->ReceivedAdapter_, this->Port_)))
	    throw new std::string("Error binding to port");

	Logger << "TFTPClient received [Port " << this->Port_ 
	       << "] for request." << Logging::endl;
	
	this->Block_ = 0;
	this->File_ = { filename, 0 };
	this->SendReadRequest();
    }	

    void
    TFTPClient::
    ReceivedAcknowledgment(TFTP::PacketType::BlockID block)
    {
	Logger << "Received acknowledgment for [Block " << block
	       << "]" << Logging::endl;
	if (block == this->Block_)
	{
	    ++(this->Block_);
	    this->Timeouts_ = 0;
	}
	this->SendData();
    }
    
    void
    TFTPClient::
    ReceivedData(TFTP::PacketType::BlockID block,
		 SizeType size)
    {
	Logger << "Received data packet of size " << size << Logging::endl;

	if (block == (this->Block_ + 1))
	{
	    ++(this->Block_);
	    this->Timeouts_ = 0;
	    this->SendAcknowledgment();

	    if (size != (TFTP::MAX_DATA + 32))
	    {
		this->File_.Size = ((this->Block_ - 1) 
				    * TFTP::MAX_DATA) + (size - (32 * Size::BIT));
		this->Terminated(TFTPClient::Completed);
	    }
	}
	else
	{
	    this->SendAcknowledgment();
	}
    }
    
    void
    TFTPClient::
    ReceivedError(TFTP::ErrorCode error)
    {
	Logger << "Received error: " << error << Logging::endl;
    }

    void
    TFTPClient::
    Received(UDPNode::MessageArg& msg)
    {
	Logger << "Received packet." << Logging::endl;
	
	if (msg.IPPkt->Sender != this->Server_)
	    throw std::string("Packet sender does not match Server IP.");

	auto tftppkt = std::static_pointer_cast<TFTP::PacketType>
	    (msg.UDPPkt->Payload);
	
	switch(tftppkt->Type)
	{
	case TFTP::Acknowledgment:
	{
	    auto block = static_cast<TFTP::Message<TFTP::Acknowledgment>*>
		(tftppkt.get())->Block;
	    this->ReceivedAcknowledgment(block);
	    break;
	}
	case TFTP::Data:
	{
	    auto ptr = static_cast<TFTP::Message<TFTP::Data>*>
		(tftppkt.get());
	    auto block = ptr->Block;
	    auto size = ptr->Size();
	    this->ReceivedData(block, size);
	    break;
	}
	case TFTP::Error:
	{
	    auto error = static_cast<TFTP::Message<TFTP::Error>*>
		(tftppkt.get())->Code;
	    this->ReceivedError(error);
	    break;
	}
	default:
	    Logger << "Received unanticipated message type: " << tftppkt->Type << Logging::endl;
	    break;
	}
    }
    
    void
    TFTPClient::
    SendReadRequest()
    {
	auto tftppkt = TFTP::PacketPtr(new ReadRequestMessage(this->File_.Name));

	this->Send(tftppkt);
    }

    void
    TFTPClient::
    SendWriteRequest()
    {
	auto tftppkt = TFTP::PacketPtr(new WriteRequestMessage(this->File_.Name));

	this->Send(tftppkt);
    }

    void
    TFTPClient::
    SendAcknowledgment()
    {
	auto tftppkt = TFTP::PacketPtr
	    (new TFTP::Message<TFTP::Acknowledgment>
	     (this->Block_));
	this->Send(tftppkt);
    }

    void
    TFTPClient::
    SendData()
    {
	SizeType remaining = 
	    this->File_.Size - ((this->Block_ - 1) * TFTP::MAX_DATA);

	Logger << "File: " << remaining << " bits remaining after " <<
	    this->Block_ << " blocks." << Logging::endl;

	if (remaining < 0)
	{
	    this->Terminated(TFTPClient::Completed);
	    return;
	}
	
	SizeType amttosend = std::min(remaining, TFTP::MAX_DATA);
	TFTP::PacketPtr tftppkt(
	    new TFTP::Message<TFTP::Data>(this->Block_, amttosend));

	this->Send(tftppkt);
    }
    
    void
    TFTPClient::
    SendError(TFTP::ErrorCode error)
    {
	TFTP::PacketPtr tftppkt(new TFTP::Message<TFTP::Error>(error));
	this->Send(tftppkt, false);
    }

    void
    TFTPClient::
    Send(TFTP::PacketPtr& pkt, bool timed)
    {
	UDP::PacketPtr udppkt = std::make_shared<UDP::PacketType>
	    (UDP::TFTP, pkt, pkt->Size(),
	     TFTP::ServerPort, this->Port_);
	this->Node_.Send(udppkt, this->Server_);
	new TransferTimer(*this, pkt->Type);
    }

    void
    TFTPClient::
    Timedout(TFTP::SubType type)
    {
	if (++(this->Timeouts_) > TIMEOUTS)
	{
	    this->Terminated(TFTPClient::TimedOut);
	    return;
	}

	switch(type)
	{
	case TFTP::ReadRequest:
	    this->SendReadRequest();
	    break;
	case TFTP::WriteRequest:
	    this->SendWriteRequest();
	    break;
	case TFTP::Acknowledgment:
	    this->SendAcknowledgment();
	    break;
	case TFTP::Data:
	    this->SendData();
	    break;
	case TFTP::Error:
	    break;
	}
    }
			      
    void
    TFTPClient::
    Terminated(TransferEventType type)
    {
	TransferEventArgs tae(*this, type, this->File_.Name, this->File_.Size);
	this->OnTransferEvent(tae);

	this->Node_.Unbind(this->Port_);
	this->Port_ = 0;
	this->Busy_ = false;
	this->Timeouts_ = 0;
	this->Block_ = 0;
    }

    void
    TFTPClient::TransferTimer::
    operator()()
    {
	if (this->Client.Block_ == this->Block && this->Client.Port_ == Port)
	    this->Client.Timedout(this->Type);
	delete this;
    }

    TFTPClient::TransferTimer::
    TransferTimer(TFTPClient& client, 
		  TFTP::SubType& type)
	: Client(client)
	, Type(type)
	, Port(client.Port_)
	, Block(client.Block_)
    {
	Simulation::Timer::Main.Schedule(TFTP::TIMEOUT, *this);
    }
}
