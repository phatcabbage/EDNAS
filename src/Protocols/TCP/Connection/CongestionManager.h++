#ifndef TCP__CONGESTIONMANAGER_HXX__
#define TCP__CONGESTIONMANAGER_HXX__

#include <cstdio>
#include <iosfwd>
#include <limits>
#include "../Protocol.h++"
#include "../ConnectionManager.h++"

namespace TCP
{
    struct CongestionManager : public ConnectionManager
    {
	static const int DEFAULT_INITIAL_CWND = 1;
	static const int CONSEC_DUPE_ACKS = 3;

	const int
	ID;

	virtual void
	Sent(SeqNumT seq){}
	
	virtual void
	Acknowledged(SeqNumT ack);
	
	virtual void
	DuplicateAcknowledged(SeqNumT ack);
	
	virtual void
	TimedOut(SeqNumT seq);

	CongestionManager(Connection& c,
			  SeqNumT cwnd=DEFAULT_INITIAL_CWND);

	virtual ~CongestionManager();

    protected:
	static int
	G_ID;

	FILE*
	Log;

	SeqNumT
	CWnd_;
	
	SeqNumT
	SSThresh_;

	SeqNumT
	CWndProgress_;
	
	int
	DupeAcks_;

	virtual void
	TripleDupe() = 0;

	void
	DeflateWindow();

	virtual void
	ClearDupes()
	{DupeAcks_ = 0; }

	// Accessors
    public:
	SeqNumT
	CongestionWindowSize() const
	{ return CWnd_; }

	void
	CongestionWindowSize(SeqNumT size)
	{ CWnd_ = size; }

	SeqNumT
	SlowStartThreshhold() const
	{ return SSThresh_; }

	void
	SlowStartThreshhold(SeqNumT n)
	{ SSThresh_ = n; }
	
	int
	DupeAcks() const
	{ return DupeAcks_; }
    };
    
    struct Tahoe : public CongestionManager
    {
	Tahoe(Connection& c, 
	      SeqNumT cwnd = DEFAULT_INITIAL_CWND)
	    : CongestionManager(c,cwnd){}

    protected:
	virtual void
	TripleDupe();

	void
	FastRetransmit();
    };

    struct Reno : public Tahoe
    {
	Reno(Connection& c, 
	      SeqNumT cwnd = DEFAULT_INITIAL_CWND)
	    : Tahoe(c,cwnd){}

	virtual void
	DuplicateAcknowledged(SeqNumT ack);

    protected:
	virtual void
	TripleDupe();

	void
	FastRecovery();

	virtual void
	ClearDupes();
    };
}

std::ostream&
operator<<(std::ostream&, const TCP::CongestionManager&);

#endif // TCP__CONGESTIONMANAGER_HXX__
