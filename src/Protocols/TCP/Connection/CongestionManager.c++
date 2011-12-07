#include <algorithm>
#include "CongestionManager.h++"
#include "../Connection.h++"
#include "Simulation/Timer.h++"

namespace TCP
{
    int
    CongestionManager::
    G_ID;

    CongestionManager::
    CongestionManager(Connection& c,
		      SeqNumT cwnd)
	: ConnectionManager(c)
	, ID(++G_ID)
	, Log()
	, CWnd_(cwnd)
	, SSThresh_(std::numeric_limits<SeqNumT>::max())
	, CWndProgress_()
	, DupeAcks_()
    {
	char buf[16];
	snprintf(buf,16,"CongAv%d.log", ID);
	Log = fopen(buf, "w");
    }
    
    CongestionManager::
    ~CongestionManager()
    {
	fclose(this->Log);
    }

    template<int n>
    struct PrintFmt
    { static const char* FmtString; };

    template<>
    const char*
    PrintFmt<4>::FmtString = "%lld\t%s\t%d\n";
	
    template<>
    const char*
    PrintFmt<8>::FmtString = "%ld\t%s\t%d\n";

    void
    CongestionManager::    
    Acknowledged(SeqNumT ack)
    {
	ClearDupes();
	
	if (CWnd_ < SSThresh_)
	{    // Slow Start
	    ++CWnd_;
	    fprintf(Log, PrintFmt<sizeof(long)>::FmtString,
		    Timer::Main.Time(), "SS", CWnd_);
	}
	else
	    // Congestion Avoidance
	    if (++CWndProgress_ == CWnd_)
	    {
		++CWnd_;
		CWndProgress_ = 0;
		fprintf(Log, PrintFmt<sizeof(long)>::FmtString,
			Timer::Main.Time(), "CA", CWnd_);

	    }
    }

    void
    CongestionManager::
    DuplicateAcknowledged(SeqNumT ack)
    {
	if(++DupeAcks_ == CONSEC_DUPE_ACKS)
	    TripleDupe();
    }
    

    void
    CongestionManager::    
    TimedOut(SeqNumT seq)
    {
	ClearDupes();

	DeflateWindow();
	CWnd_ = 1;
    }

    void
    CongestionManager::
    DeflateWindow()
    {
	// RFC2001 2.3
	SeqNumT curWnd = std::min(CWnd_, C_.RWS_.RWND);
	SSThresh_ = std::max(2, curWnd >> 1);
    }	

    void
    Tahoe::
    TripleDupe()
    {
	DeflateWindow();
	fprintf(Log, PrintFmt<sizeof(long)>::FmtString,
		Timer::Main.Time(), "TRIPDUP", CWnd_);
	FastRetransmit();
	CWnd_ = 1; // <-- _NOT_ fast recovery
    }
    
    void
    Tahoe::
    FastRetransmit()
    {
	if(C_.Transfers_.empty()) return;

	SeqNumT seq = C_.RWS_.ACK;
	C_.ResendPacket(seq);
    }
    
    void
    Reno::
    DuplicateAcknowledged(SeqNumT ack)
    {
	++DupeAcks_;
	if(DupeAcks_ < CONSEC_DUPE_ACKS) return;
	if(DupeAcks_ > CONSEC_DUPE_ACKS)
	{
	    ++CWnd_;
	    C_.Send();
	}
	else
	{
	    TripleDupe();
	}
    }

    void
    Reno::
    TripleDupe()
    {
	Tahoe::TripleDupe();
	FastRecovery();
    }
	

    void
    Reno::
    FastRecovery()
    {
	CWnd_ = SSThresh_ + CONSEC_DUPE_ACKS;
	C_.Send();
    }
    
    void
    Reno::
    ClearDupes()
    {
	if (DupeAcks_ >= CONSEC_DUPE_ACKS)
	    CWnd_ = SSThresh_;
	DupeAcks_ = 0;
    }
}
