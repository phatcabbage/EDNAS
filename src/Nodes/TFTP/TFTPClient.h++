#ifndef NODES__TFTPCLIENT_HXX__
#define NODES__TFTPCLIENT_HXX__

#include <string>

#include "Core/Signaling/EventAdapter.h++"
#include "Core/Signaling/SingleEvent.h++"
#include "Logging/Logged.h++"

#include "Protocols/UDP.h++"
#include "Protocols/TFTP.h++"

#include "UDPNode.h++"

namespace Nodes
{
    using Protocols::TFTP;
    using Protocols::UDP;

    struct TFTPClient : public Logging::Logged<TFTPClient>
    {

	struct TransferEventArgs;
	enum TransferEventType
	{
	    Completed,
	    TimedOut,
	    Error
	};
	
	static const int TIMEOUTS = 10;

	static UDP::Port
	RandomPort();
    
	SingleEvent<TransferEventArgs&>
	OnTransferEvent;

	UDPNode&
	Node()
	{ return this->Node_; }
    
	bool
	IsBusy() const
	{ return this->Busy_; }

	TimeType
	Timeout() const
	{ return this->Timeout_;}
    
	UDP::AddressType
	Port() const
	{ return this->Port_; }

	IPv4::AddressType
	Server() const
	{ return this->Server_; }
	
	void
	Server(IPv4::AddressType ip)
	{ if (!this->Busy_) this->Server_ = ip; }

	void
	SendFile(const std::string& filename,
		 SizeType size);
	
	void
	RequestFile(const std::string& filename);

	TFTPClient(UDPNode& node,
		   TimeType timeout = TFTP::TIMEOUT);

	virtual 
	~TFTPClient();

    protected:
	UDPNode& 
	Node_;

	EventAdapter<TFTPClient, UDPNode::MessageArg&>
	ReceivedAdapter_;

	bool
	Busy_;
	
	TimeType
	Timeout_;

	int
	Timeouts_;

	UDP::AddressType
	Port_;
    
	IPv4::AddressType
	Server_;
    
	TFTP::PacketType::BlockID
	Block_;

	TFTP::File
	File_;

	void
	ReceivedAcknowledgment(TFTP::PacketType::BlockID block);
    
	void
	ReceivedData(TFTP::PacketType::BlockID block, SizeType size);
    
	void
	ReceivedError(TFTP::ErrorCode error);
    
	void
	Received(UDPNode::MessageArg& msg);

	void
	SendWriteRequest();
	
	void
	SendReadRequest();

	void
	SendAcknowledgment();
	
	void
	SendData();
	
	void
	SendError(TFTP::ErrorCode error);

	void
	Send(TFTP::PacketPtr& pkt, bool timed = true);

	void
	Timedout(TFTP::SubType type);

	void
	Terminated(TransferEventType type);

	struct TransferTimer;
    };

    struct TFTPClient::TransferEventArgs
    {
	TFTPClient&		Client;
	TransferEventType	Type;
	std::string		FileName;
	SizeType		Size;

	TransferEventArgs(TFTPClient& client, TransferEventType event,
			  std::string filename, SizeType size)
	    : Client(client), Type(event), FileName(filename), Size(size)
	{}
    };

    struct TFTPClient::TransferTimer
    {
	TFTPClient&	Client;
	TFTP::SubType	Type;
	UDP::Port	Port;
	TFTP::PacketType::BlockID Block;
	void operator()();
	TransferTimer(TFTPClient& client, 
		      TFTP::SubType& type);
    };
}

#endif // NODES__TFTPCLIENT_HXX__
