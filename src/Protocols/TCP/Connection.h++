#ifndef TCP__CONNECTION_HXX__
#define TCP__CONNECTION_HXX__

#include <list>

#include "SEDNAS.h++"
#include "Simulation/Event.h++"
#include "Protocol.h++"
#include "Packet.h++"
#include "Connection/Base.h++"


namespace TCP
{
    struct Connector;
    struct ConnectionManager;
    struct CongestionManager;
    struct DebugManager;
    struct ResultsManager;
    struct RTTManager;
    struct TimerManager;
    struct TransferManager;

    struct Connection : public _Connection::Base,
			public EventHandler<Network::PacketListPtr&>
    {
	Connection(Connector& c, Socket remote = Socket());
	
	virtual void 
	Received(Network::PacketListPtr& pktList);

	Socket
	LocalSocket() const;

	virtual void
	Send(DataPtr data);

	Event<Message&>&
	OnReceive()
	{ return this->OnReceive_; }

	void
	Connect();

	virtual void
	operator()(Network::PacketListPtr& pktList)
	{ this->Received(pktList); }

	virtual ~Connection();

    protected:
	typedef std::list<PacketPtr> BufferType;
	struct TransferRecord
	{
	    PacketPtr	Pkt;
	    int		Count;
	    TimeType	Sent;
	    TimeType	Ack;
	};	    
	struct QueuedMessage
	{
	    DataPtr Ptr;
	    SizeType Size;
	};
	typedef std::list<QueuedMessage> MessageQueue;

	typedef std::map<SeqNumT, TransferRecord>
	RecordContainer;

	Connector& 
	Conn_;

	Socket
	RemoteSocket_;
	
	std::unique_ptr<CongestionManager>
	CongMgr_;

	std::unique_ptr<DebugManager>
	DebugMgr_;

	std::unique_ptr<ResultsManager>
	ResultsMgr_;

	std::unique_ptr<RTTManager>
	RTTMgr_;

	std::unique_ptr<TimerManager>
	TimerMgr_;

	WindowStatus
	LWS_;
	  
	WindowStatus
	RWS_;

	MessageQueue
	QueuedMessages_;

	BufferType
	OutBuffer_;

	BufferType
	InFlight_;

	RecordContainer
	Transfers_;

	std::map<SeqNumT,PacketPtr>
	InBuffer_;

	/* Debug */
//	RecordContainer
//	Completed_;
	/* End Debug */

	std::map<SeqNumT,DataPtr>
	DataStream_;

	SingleEvent<Message&>
	OnReceive_;

	SeqNumT
	NextUnallocatedSeqNum_;

	bool
	AcknowledgeNeeded_;

	void
	CreateSYNPacket();

	virtual void
	ProcessSynchronizeFlag(const PacketPtr& pkt);
	
	virtual void
	ProcessAcknowledgeFlag(const PacketPtr& pkt);
	
	virtual void
	ProcessDataFlag(const PacketPtr& pkt);

	virtual void
	ProcessCloseFlag(const PacketPtr& pkt);

	void
	Connected();

	void
	AcceptConnection();

	bool
	IsInWindow(SeqNumT seqnum) const;

	void
	Acknowledged(SeqNumT ack);

	void
	DuplicateAcknowledgment(SeqNumT ack);

	void
	TimedOut();

	void
	Send();
	
	void
	SendPacket();

	void 
	ResendPacket(SeqNumT seq);

	void
	Acknowledge();

	void
	ReceivedData(const PacketPtr pkt);

	void
	Drop(Network::PacketListPtr& pktList, int reason = 0);

	Connection(const Connection&);

	void
	SipQueue();

	void
	BufferPackets(std::list<PacketPtr>& pkts);

	// Accessors
    public:
	Connector&
	Conn()
	{ return Conn_; }
	
	Socket
	RemoteSocket() const
	{ return RemoteSocket_; }

	IPv4::Protocol::AddressType
	RemoteIP() const
	{ return RemoteSocket_.IP; }
	
	Protocol::AddressType
	RemotePort() const
	{ return RemoteSocket_.Port; }

	// CongestionManager*
	// CongestionMgr() const
	// { return CongMgr_.get(); }

	// void
	// CongestionMgr
	// (CongestionManager* cmgr);
	
	WindowStatus
	LocalWindowStatus() const
	{ return LWS_; }
	
	WindowStatus
	RemoteWindowStatus() const
	{ return RWS_; }

	friend class CongestionManager;
	friend class DebugManager;
	friend class TimerManager;
	friend class RTTManager;
	friend class Tahoe;
	friend class Reno;	
    };
}

std::ostream&
operator<<(std::ostream&, const TCP::Connection&);

std::ostream&
operator<<(std::ostream&, const TCP::Connection::WindowStatus&);

#endif // TCP__CONNECTION_HXX__
