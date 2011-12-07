#ifndef NODES__TFTPSERVER_HXX__
#define NODES__TFTPSERVER_HXX__

#include <map>
#include <string>

#include "Core/Signaling/EventAdapter.h++"
#include "Logging/Logged.h++"
#include "Protocols/UDP.h++"
#include "Protocols/TFTP.h++"

#include "UDPNode.h++"

namespace Nodes
{
    using Protocols::TFTP;

    struct TFTPServer : public Logging::Logged<TFTPServer>
    {
	static const int TIMEOUTS = 10;
	
	UDPNode&
	Node()
	{ return this->Node_; }

	bool
	ReadOnly() const
	{ return this->ReadOnly_; }

	TimeType
	Timeout() const
	{ return this->Timeout_; }

	void
	AddFile(const std::string& name, SizeType size)
	{ this->Files_[name] = { name, size };}

	void
	ReadOnly(bool ro)
	{ this->ReadOnly_ = ro; }

	void
	Timeout(TimeType time)
	{ this->Timeout_ = time; }

	TFTPServer(UDPNode& node,
		   bool readonly  = true,
		   TimeType timeout = double(TFTP::TIMEOUT))
	    : Node_(node)
	    , ReceivedAdapter_(*this, &TFTPServer::Received)
	    , Files_()
	    , ReadOnly_(readonly)
	    , Timeout_(timeout)
	{
	    node.Bind(this->ReceivedAdapter_, TFTP::ServerPort);
	}

	virtual ~TFTPServer()
	{
	    Node_.Unbind(TFTP::ServerPort);
	}

    protected:
	struct Transfer;
	
	UDPNode& 
	Node_;
	
	EventAdapter<TFTPServer, UDPNode::MessageArg&>
	ReceivedAdapter_;
	
	std::map<std::string, TFTP::File>
	Files_;
	
	std::map<UDP::Port, Transfer>
	Outgoing_;
	
	std::map<UDP::Port, Transfer>
	Incoming_;

	bool
	ReadOnly_;
	
	TimeType
	Timeout_;

	void
	Received(UDPNode::MessageArg& msg);

	void
	ReceivedReadRequest(IPv4::AddressType ip,
			    UDP::AddressType port,
			    const std::string& filename);
	
	void
	ReceivedWriteRequest(IPv4::AddressType ip,
			     UDP::AddressType port,
			     const std::string& filename);
	
	void
	ReceivedAcknowledgment(IPv4::AddressType ip,
			       UDP::AddressType port,
			       TFTP::PacketType::BlockID block);

	void
	ReceivedData(IPv4::AddressType ip,
		     UDP::AddressType port,
		     TFTP::PacketType::BlockID block,
		     SizeType size);
	
	void
 	ReceivedError(IPv4::AddressType ip,
		      UDP::AddressType port,
		      TFTP::ErrorCode error);

	void
	SendError(TFTP::ErrorCode error, IPv4::AddressType ip, UDP::AddressType port);

	void
	SendData(Transfer& t);

	void
	SendAcknowledgment(Transfer& t);

	void
	IncomingTimedOut(Transfer& t);
	
	void
	OutgoingTimedOut(Transfer& t);

	void
	IncomingCompleted(Transfer& t);
	
	void
	OutgoingCompleted(Transfer& t);

	struct TransferTimer;
    };

//     struct TFTPServer::MessageArg
//     {
// 	IPv4::PacketPtr IPPkt;
// 	UDP::PacketPtr  UDPPkt;
// 	TFTP::PacketPtr TFTPPkt;
// 	MessageArg(IPv4::PacketPtr& ippkt,
// 		   UDP::PacketPtr&  udppkt,
// 		   TFTP::PacketPtr& tftppkt)
// 	    : IPPkt(ippkt)
// 	    , UDPPkt(udppkt)
// 	    , TFTPPkt(tftppkt)
// 	{}
//     };
	

    struct TFTPServer::Transfer
    {
	TFTP::File
	File;

	IPv4::AddressType
	IP;

	UDP::AddressType
	Port;

	TFTP::PacketType::BlockID
	LastACK;

	int 
	Timeouts; /* consecutive */
    };

    struct TFTPServer::TransferTimer
    {
	enum DirectionEnum
	{
	    Incoming,
	    Outgoing
	};
	
	TFTPServer&
	Server;
	
	UDP::AddressType
	Port;

	const
	DirectionEnum
	Direction;
	
	const 
	TFTP::PacketType::BlockID 
	LastACK;
	
	void operator()()
	{
	    switch(Direction)
	    {
	    case (Incoming):
	    {
		if (!Server.Incoming_.count(Port)) return;
		Transfer& t = Server.Incoming_[Port];
		if (t.LastACK != this->LastACK) return;
		Server.IncomingTimedOut(t);
		break;
	    }
	    case (Outgoing):
		if (!Server.Outgoing_.count(Port)) return;
		Transfer& t = Server.Outgoing_[Port];
		if (t.LastACK != this->LastACK) return;
		Server.OutgoingTimedOut(t);
		break;
	    }
	    delete this;
	}

	TransferTimer(TFTPServer& server,
		      UDP::AddressType port,
		      DirectionEnum direction,
		      TFTP::PacketType::BlockID lastack)
	    : Server(server)
	    , Port(port)
	    , Direction(direction)
	    , LastACK(lastack)
	{}
    };
}

#endif // NODES__TFTPSERVER_HXX__
