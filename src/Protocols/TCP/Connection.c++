#include <cassert>
#include <algorithm>
#include <utility>

#include "Simulation/Timer.h++"
#include "Logging/OStreamLogger.h++"
#include "Connection.h++"
#include "Connector.h++"
#include "Connection/CongestionManager.h++"
#include "Connection/DebugManager.h++"
#include "Connection/ResultsManager.h++"
#include "Connection/RTTManager.h++"
#include "Connection/TimerManager.h++"

namespace TCP
{
    static
    Logging::OStreamLogger
    Log("<TCP Connection>");

    static
    Logging::OStreamLogger
    PacketLog("<TCP Packet>");

    bool
    IsData(PacketPtr& pkt)
    { 
	const static int DataMask = (Protocol::Synchronize ||
				     Protocol::Data);
	return pkt->Flags & DataMask;
    }

    enum PacketDropReasons
    {
	UNSPECIFIED,
	OUTOFWINDOW,
	WRONGADDRESS,
	MISPLACED_SYN,
	UNDELIVERABLE
    };

    const static size_t MAX_OUTBUFFER_SIZE = 512;
    const static size_t COMFY_OUTBUFFER_SIZE = 64;

    Connection::
    Connection(Connector& c,
	       Socket remote)
	: EventHandler<Network::PacketListPtr&>()
	, Conn_(c)
	, RemoteSocket_(remote)
	, CongMgr_()
	, DebugMgr_()
	, ResultsMgr_()
	, RTTMgr_()
	, TimerMgr_()
	, LWS_({0,0,DEF_LOCAL_RCV_WND})
	, RWS_({0,0,DEF_REMOTE_RCV_WND})
	, QueuedMessages_()
	, OutBuffer_()
	, InFlight_()
	, Transfers_()
	, InBuffer_()
//	, Completed_() /* DEBUG */
	, DataStream_()
	, OnReceive_()
	, NextUnallocatedSeqNum_(0)
	, AcknowledgeNeeded_(false)
    {
	CongMgr_   .reset(new Reno(*this));
	DebugMgr_  .reset(new DebugManager(*this));
	ResultsMgr_.reset(new ResultsManager(*this));
	RTTMgr_    .reset(new RTTManager(*this));
	TimerMgr_  .reset(new TimerManager(*this));

	CreateSYNPacket();
    }

    Connection::
    ~Connection()
    {
	Conn_.Closed(RemoteSocket_);
    }

    void
    Connection::
    CreateSYNPacket()
    {
	PacketPtr p = 
	    std::make_shared<Packet>(Protocol::Synchronize);
	p->SEQ = NextUnallocatedSeqNum_++;
	OutBuffer_.push_back(p);
	ResultsMgr_->Created(p->SEQ);
	DebugMgr_->Created(p->SEQ);
    }

    void
    Connection::
    Send(DataPtr data)
    {
	SizeType size = data->Size();
	size_t pkts = (size / Protocol::DEF_MAX_SEGMENT_SIZE) + 1;

	if (!size) return;
	
	if((OutBuffer_.size() + pkts) > MAX_OUTBUFFER_SIZE)
	{
	    QueuedMessage qm = { data, data->Size() };
	    QueuedMessages_.push_back(qm);
	}
	else
	{
	    std::list<PacketPtr> packets = Packet::Wrap(data);
	    
	    for(auto it = packets.begin(), end = packets.end();
		it != end; ++it)
	    {
		SeqNumT seq = NextUnallocatedSeqNum_++;
		(*it)->SEQ = seq;
		ResultsMgr_->Created(seq);
		
		// PacketLog << "Packet [SEQ " << (*it)->SEQ 
		// 	      << "] created." << Logging::endl;
	    }
	    
	    // BufferType::iterator dbgIt = OutBuffer_.end();
	    // --dbgIt;
	    OutBuffer_.splice(OutBuffer_.end(), packets);
	    // ++dbgIt; /* I hope this points to the new elements... */
	    // for(;dbgIt != OutBuffer_.end(); ++dbgIt)
	    //     DebugMgr_->Created((*dbgIt)->SEQ);
	}

	Send();
    }

    void
    Connection::
    Received(Network::PacketListPtr& pktList)
    {
	auto pkt = std::static_pointer_cast<Packet>
	    ((*pktList)[Protocol::Layer]);

	// PacketLog << *this << " Received packet [SEQ "
	// 	  << pkt->SEQ << "]" 
	// 	  << Logging::endl;

	int flags = pkt->Flags;

	/* Verify packet is in window */	
	if (!IsInWindow(pkt->SEQ))
	{
	    Drop(pktList, PacketDropReasons::OUTOFWINDOW);
	    return;
	}

	if (flags & Protocol::Synchronize)
	{
	    ProcessSynchronizeFlag(pkt);
	}
	    
	if (flags & Protocol::Acknowledge)
	{
	    ProcessAcknowledgeFlag(pkt);
	}

	if (flags & Protocol::Data)
	{
	    ProcessDataFlag(pkt);
	}
	
	// if (flags & (Protocol::Data | Protocol::Synchronize))
	//     assert(AcknowledgeNeeded_);

	Send();
    }

    void
    Connection::
    ProcessSynchronizeFlag(const PacketPtr& pkt)
    {
	AcknowledgeNeeded_ = true;
	if (LWS_.SEQ == 0)
	    AcceptConnection();

	if(RWS_.SEQ <= pkt->SEQ)
	    RWS_.SEQ = pkt->SEQ + 1;
    }

    void
    Connection::
    ProcessAcknowledgeFlag(const PacketPtr& pkt)
    {
	// assert(RWS_.ACK ==
	//        OutBuffer_.front()->ACK);
	// assert(RWS_.ACK ==
	//        Transfers_.begin()->second.P->
	//        ACK);

	SeqNumT acknum = pkt->ACK;

	if(acknum > RWS_.ACK)
	    Acknowledged(acknum);
	else if(acknum == RWS_.ACK)
	    DuplicateAcknowledgment(acknum);
	else
	    Log << "ERROR: We received an ack for a long-"
		<< "gone packet. Make sure this doesn't "
		<< "happen frequently."
		<<  Logging::endl;
    }

    void
    Connection::
    ProcessDataFlag(const PacketPtr& pkt)
    {
	AcknowledgeNeeded_ = true;
	SeqNumT seqnum = pkt->SEQ;

	// Next packet in sequence: PKT.SEQ = RWND.SEQ
	// Future packets: PKT.SEQ > RWND.SEQ

	// is this a duplicate?
	if(seqnum < RWS_.SEQ) return;
	// or out of order?
	else if(seqnum > RWS_.SEQ)
	{
	    Log << *this << " received data out of order.\n"
		<< "\tRMT.SEQ = " << RWS_.SEQ
		<< "\tPKT.SEQ = " << pkt->SEQ
		<< Logging::endl;
	    
	    InBuffer_[seqnum] = pkt;
	}
	// nope, just in time
	else
	    ReceivedData(pkt);
    }

    void
    Connection::
    ProcessCloseFlag(const PacketPtr& pkt)
    {}

    Socket
    Connection::
    LocalSocket() const
    {
	return Conn_.LocalSocket();
    }

    void
    Connection::
    Connect()
    {
	Log << *this << " attempting connection." 
	    << Logging::endl;

	/* DEBUG */
	if (LWS_.SEQ != 0)
	{
	    Log << *this 
		<< " attempted to connect when SEQ != 0"
		<< Logging::endl;

	    return;
	}
	/* END DEBUG */

	Send();
    }

    void
    Connection::
    Connected()
    {
	Log << *this 
	    << " reports successful connection!" 
	    << Logging::endl;
    }

    void
    Connection::
    AcceptConnection()
    {
	Log << *this 
	    << " accepting connection." 
	    << Logging::endl;
    }

    bool
    Connection::
    IsInWindow(SeqNumT seqnum) const
    {
	return (seqnum <= (RWS_.SEQ + LWS_.RWND));
    }

    void
    Connection::
    Acknowledged(SeqNumT ack)
    {
	// PacketLog << "Acknowledged [SEQ " << ack << "]" 
	// 	  << Logging::endl;

	SeqNumT seq = ack - 1;
	/* DEBUG */
	// have we sent it yet?
	assert(ack <= NextUnallocatedSeqNum_);
	// make sure it's not a dupe
	assert(ack >  RWS_.ACK); 
	// and that it's in transfers
	assert(Transfers_.count(seq));
	/* END DEBUG */

	auto it = Transfers_.find(seq);
	assert(it != Transfers_.end());
//	it = Completed_.insert(*it).first; // DEBUG
	it->second.Ack = Timer::Main.Time();
	
	RWS_.ACK = ack;

	auto b = Transfers_.begin(), 
	    e = Transfers_.upper_bound(seq);
	
	TimeType time = Timer::Main.Time();

	for(auto i = b; i != e; ++i)
	    i->second.Ack = time;

//	Completed_.insert(b, e); // DEBUG
	RTTMgr_    ->Acknowledged(ack);
	Transfers_.erase(b, e);

	while(!InFlight_.empty() &&
	      InFlight_.front()->SEQ <= seq)
	    InFlight_.pop_front();

	while(!OutBuffer_.empty() && 
	      OutBuffer_.front()->SEQ <= seq)
	    OutBuffer_.pop_front();

	if(!QueuedMessages_.empty() &&
	   (OutBuffer_.size() < COMFY_OUTBUFFER_SIZE))
	{ SipQueue(); }

	DebugMgr_  ->Acknowledged(ack);
	ResultsMgr_->Acknowledged(ack);
	TimerMgr_  ->Acknowledged(ack);
	CongMgr_   ->Acknowledged(ack);
    }

    void
    Connection::
    DuplicateAcknowledgment(SeqNumT ack)
    {
	SeqNumT seq = ack - 1;
	/* DEBUG */
	// have we sent it yet?
	assert(ack <= NextUnallocatedSeqNum_); 
	// are we sure it's a dupe?
	assert(ack <= RWS_.ACK); 
	// it shouldn't be in the current transfers either
	assert(Transfers_.count(seq) == 0);
	/* END DEBUG */

	ResultsMgr_->DuplicateAcknowledged(ack);
	DebugMgr_->DuplicateAcknowledged(ack);
	CongMgr_->DuplicateAcknowledged(ack);

	// assert(IsInOrder(OutBuffer_));
	// assert(IsInOrder(InFlight_));	

    }

    void
    Connection::
    TimedOut()
    {
	/* BOOM! */
	assert(!InFlight_.empty());
	assert(InFlight_.front()->SEQ == RWS_.ACK);
	
	OutBuffer_.splice(OutBuffer_.begin(), InFlight_);
	LWS_.SEQ = OutBuffer_.front()->SEQ;
	SeqNumT seq = LWS_.SEQ;
	assert(RWS_.ACK == LWS_.SEQ);
	
	RTTMgr_->TimedOut(seq);
	CongMgr_->TimedOut(seq);
	ResultsMgr_->TimedOut(seq);
	
	// assert(IsInOrder(OutBuffer_));
	// assert(IsInOrder(InFlight_));	

	Send();
    }	
    
    void
    Connection::
    Send()
    {
	// Log << *this << " beginning Send()."
	//     << Logging::endl;

	if(!QueuedMessages_.empty() &&
	   (OutBuffer_.size() < COMFY_OUTBUFFER_SIZE))
	    SipQueue();
	
	SeqNumT wnd = 
	    std::min(RWS_.RWND, 
		     CongMgr_->CongestionWindowSize());

	while(InFlight_.size() < (size_t)wnd)
	{
	    if(OutBuffer_.empty())
	    {
		if(!QueuedMessages_.empty())
		    SipQueue();
		else
		    break;
	    }
	    
	    SendPacket();
	}


	if (AcknowledgeNeeded_) Acknowledge();

	// assert(IsInOrder(OutBuffer_));
	// assert(IsInOrder(InFlight_));	
    }

    void
    Connection::
    ResendPacket(SeqNumT seq)
    {
//	assert(!Completed_.count(seq));
	
	auto it = Transfers_.find(seq);
	assert(it != Transfers_.end());

	++(it->second.Count);
	it->second.Sent = Timer::Main.Time();
	
	PacketPtr pkt = 
	    std::make_shared<Packet>(*(it->second.Pkt));
	
	pkt->ACK = RWS_.SEQ;
	if(LWS_.ACK < RWS_.SEQ)
	{
	    AcknowledgeNeeded_ = false;
	    pkt->Flags |= Protocol::Acknowledge;
	    LWS_.ACK = RWS_.SEQ;
	}
	
	auto pktList = 
	    std::make_shared<Network::PacketList>();
	
	(*pktList)[Protocol::Layer] = pkt;

	ResultsMgr_->Sent(pkt->SEQ);
	Conn_.Send(pktList, RemoteSocket_.IP);
    }

    void
    Connection::
    SendPacket()
    {
	PacketPtr pkt = std::make_shared<Packet>
	    (*(OutBuffer_.front()));

	InFlight_.splice(InFlight_.end(), OutBuffer_, 
			 OutBuffer_.begin());

	/* DEBUG */
	assert(pkt);
	/* END DEBUG */

	SeqNumT seqnum = pkt->SEQ;

	pkt->Receiver = RemoteSocket_.Port;

	LWS_.SEQ = seqnum + 1;

	pkt->ACK = RWS_.SEQ;

	// Acknowledge any pending remote data
	if (AcknowledgeNeeded_)
	{
	    pkt->Flags |= Protocol::Acknowledge;
	    LWS_.ACK =
		RWS_.SEQ;
	    AcknowledgeNeeded_ = false;
	}

	auto pktList = 
	    std::make_shared<Network::PacketList>();

	(*pktList)[Protocol::Layer] = pkt;

	/* Debug */
	PacketLog << *this << "Sending packet:\t"
		  << "[SEQ: " << pkt->SEQ << "]"
		  << "[ACK: " << pkt->ACK << "]";
	if(pkt->Flags & Protocol::Synchronize)
	    PacketLog << "(SYN)";
	if(pkt->Flags & Protocol::Acknowledge)
	    PacketLog << "(ACK)";
	if(pkt->Flags & Protocol::Data)
	    PacketLog << "(DATA)";
	PacketLog << Logging::endl;
	/* End Debug */

	TimerMgr_->Sent(seqnum);

	ResultsMgr_->Sent(pkt->SEQ);
	Conn_.Send(pktList, RemoteSocket_.IP);

	auto trIt = Transfers_.find(seqnum);
	if(trIt == Transfers_.end())
	{
	    Transfers_[seqnum] = 
		{ InFlight_.back(), 
		  1,
		  Timer::Main.Time(),
		  0
		};
	}
	else
	{
	    TransferRecord& tr = trIt->second;
	    tr.Sent = Timer::Main.Time();
	    ++tr.Count;
	}

	DebugMgr_->Sent(pkt->SEQ);
    }

    void
    Connection::
    Acknowledge()
    {
	auto pkt = std::make_shared<Packet>
	    (Protocol::Acknowledge);

	auto pktList = std::make_shared<Network::PacketList>();
	(*pktList)[Protocol::Layer] = pkt;

	pkt->SEQ = LWS_.SEQ;
	pkt->ACK = RWS_.SEQ;
	     
	pkt->Receiver = RemoteSocket_.Port;

	LWS_.ACK = RWS_.SEQ;
	
	AcknowledgeNeeded_ = false;

	Conn_.Send(pktList, RemoteSocket_.IP);
    }

    void
    Connection::
    ReceivedData(const PacketPtr pkt)
    {
	RWS_.SEQ = pkt->SEQ + 1;

	if (pkt->HasData)
	{
	    DataPtr data = pkt->Data;
	    Message m(*this, data);
	    this->OnReceive_(m);
	}

	while(!InBuffer_.empty() && 
	      InBuffer_.begin()->first == RWS_.SEQ)
	{
	    ++(RWS_.SEQ);

	    if(pkt->HasData)
	    {
		DataPtr data = InBuffer_.begin()->second->Data;
		Message m(*this, data);
		this->OnReceive_(m);
	    }
	    
	    InBuffer_.erase(InBuffer_.begin());
	}
    }

    void
    Connection::
    Drop(Network::PacketListPtr& pktList, int reason)
    {
	static const char* ReasonMsg[] =
	    {
		"Unspecified", 
		"Sequence number out of receive window.",
		"Not addressed to us.", 
		"Unanticipated synchronization request."
	    };
	PacketLog << *this << " dropping packet " 
		  << "(Reason: " << ReasonMsg[reason] << ")"
		  << Logging::endl;
    }

    void
    Connection::
    SipQueue()
    {
	size_t pktCount = OutBuffer_.size();
	if(pktCount >= MAX_OUTBUFFER_SIZE) return;

	size_t spaceLeft = MAX_OUTBUFFER_SIZE - pktCount;
	
	while(spaceLeft > 0 && !QueuedMessages_.empty())
	{
	    QueuedMessage& qm = *(QueuedMessages_.begin());
	    SizeType& msgLeft = qm.Size;
	    size_t msgPackets = (msgLeft / Protocol::DEF_MAX_SEGMENT_SIZE) + 1;
	    std::list<PacketPtr> buf;
	    if(msgPackets <= spaceLeft)
	    {
		buf = Packet::Wrap(qm.Ptr);
		msgLeft = 0;
		QueuedMessages_.pop_front();
		spaceLeft -= buf.size();
	    }
	    else
	    {
		buf.resize(spaceLeft);

		for(auto it = buf.begin(), end = buf.end();
		    it != end; ++it)
		{
		    it->reset
			(new Packet(Protocol::Data, 0, 0,
				    Protocols::FAKE, 0, 0,
				    Protocol::DEF_MAX_SEGMENT_SIZE));
		    (*it)->Data = qm.Ptr;
		}
		
		SizeType bufSize = spaceLeft * Protocol::DEF_MAX_SEGMENT_SIZE;
		assert(bufSize <= msgLeft);
		msgLeft -= bufSize;
		spaceLeft = 0;
	    }	    
	    
	    for(auto it = buf.begin(), end = buf.end();
		it != end; ++it)
	    {
		SeqNumT seq = NextUnallocatedSeqNum_++;
		(*it)->SEQ = seq;
		ResultsMgr_->Created(seq);
	    }
	 
	    OutBuffer_.splice(OutBuffer_.end(), buf);
	}
    }

    namespace _Connection
    {
	const SeqNumT
	Base::DEF_LOCAL_RCV_WND;

	const SeqNumT
	Base::DEF_REMOTE_RCV_WND;
    
	const int
	Base::DEF_MAX_TIMEOUTS;
    }
}

std::ostream&
operator<<(std::ostream& o, const TCP::Connection& c)
{ 
    o << "[Connection (" << c.LocalSocket() << ") -> ("
      << c.RemoteSocket() << ")] ";
    return o;
}

std::ostream&
operator<<(std::ostream& o, const TCP::Connection::WindowStatus& ws)
{
    return o << "[[Window Status] SEQ: " << ws.SEQ << " "
	     << "ACK: " << ws.ACK << " " 
	     << "RWND: " << ws.RWND << "] ";
}

std::ostream&
operator<<(std::ostream& o, const TCP::CongestionManager& caa)
{
    return o << "[[Congestion Manager] cwnd: " 
	     << caa.CongestionWindowSize() << " ssthresh: " 
	     << caa.SlowStartThreshhold() << "] ";
}
