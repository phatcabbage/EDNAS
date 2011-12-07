#include <algorithm>
#include <iostream>
#include "Logging/OStreamLogger.h++"
#include "RTTManager.h++"
#include "../Connection.h++"
#include "Simulation/Timer.h++"

namespace TCP
{
    // static
    // Logging::OStreamLogger
    // RTTLog("RTTManager");

    const TimeType 
    RTTManager::INVALID_TIME;

    const TimeType
    RTTManager::DEFAULT_INITIAL_TIMEOUT;

    const int 
    RTTManager::DEFAULT_LOG_INV_ALPHA;

    const int 
    RTTManager::DEFAULT_LOG_INV_BETA;
    
    const int
    RTTManager::DEFAULT_LOG_K;

    int
    RTTManager::G_ID;

    RTTManager::
    RTTManager(Connection& c, int a, int b, int k)
	: ConnectionManager(c)
	, ID(++G_ID)
	, Log()
	, Alpha_(a)
	, Beta_(b)
	, K_(k)
	, Smoothed_(INVALID_TIME)
	, Variance_()
	, Timeout_(DEFAULT_INITIAL_TIMEOUT)
    {
	char namebuf[16];
	snprintf(namebuf, 16, "RTTMan%d.log", ID);
	Log = fopen(namebuf, "w");
    }

    RTTManager::
    ~RTTManager()
    {
	fclose(this->Log);
    }

    void
    RTTManager::
    Acknowledged(SeqNumT ack)
    {
	SeqNumT seq = ack - 1;
	Connection::TransferRecord& tr =
	    C_.Transfers_[seq];
	if(tr.Count == 1)
	{
	    TimeType rtt = tr.Ack - tr.Sent;
	    Acknowledged(rtt);
	}
    }

    void
    RTTManager::
    TimedOut(SeqNumT seq)
    {
	TimedOut();
    }

    template<int n>
    struct PrintFmter
    { static const char* FmtString; };

    template<>
    const char*
    PrintFmter<4>::FmtString = "%lld\t%s\t%lld\n";
	
    template<>
    const char*
    PrintFmter<8>::FmtString = "%ld\t%s\t%ld\n";

    void
    RTTManager::
    Acknowledged(TimeType rtt)
    {
	// RTTLog << *this << " Acknowledged." 
	//        << Logging::endl;
	if (Smoothed_ == INVALID_TIME)
	{
	    // This is only used for calculating the 
	    // initial values.
	    // RFC 2988 Equation 2.2
	    Smoothed_ = rtt;
	    Variance_ = rtt >> 1;
	}
	else
	{
	    // Used for all subsequent calculations.
	    // RFC 2988 Equation 2.3

	    TimeType absdiff = Smoothed_ - rtt;
	    if (absdiff < 0) absdiff = -absdiff;

	    Variance_ -= (Variance_ >> Beta_);
	    Variance_ += (absdiff >> Beta_);
	    
	    Smoothed_ -= (Smoothed_ >> Alpha_);
	    Smoothed_ += (rtt >> Alpha_);
	}

	// Equation 2.4
	Timeout_ = std::max(Smoothed_ + (Variance_ << K_),
			    Time::SECOND);

	fprintf(this->Log,PrintFmter<sizeof(long)>::FmtString,
		Timer::Main.Time(), "UPDATE", Timeout_);
	// RTTLog << *this << "\tUpdated." << Logging::endl;
    }

    void
    RTTManager::
    TimedOut()
    {
	// RTTLog << *this << " Timed out." << Logging::endl;

	// Equation 5.5
	Timeout_ = std::max(1 * Time::SECOND, Timeout_ << 1);
	// RTTLog << *this << "\tUpdated." << Logging::endl;
	fprintf(this->Log,PrintFmter<sizeof(long)>::FmtString,
		Timer::Main.Time(), "TIMEOUT", Timeout_);
    }
    
    void
    RTTManager::
    Alpha(int i)
    { Alpha_ = i; }

    void
    RTTManager::
    Beta(int i)
    { Beta_ = i; }

    void
    RTTManager::
    K(int i)
    { K_ = i; }
}



std::ostream&
operator<<(std::ostream& o, const TCP::RTTManager& r)
{ 
    return o << "[[RTT Manager #" << r.ID
	     << "] SRTT: " << r.SmoothedRTT() 
	     << " VAR: " << r.Variance() << " TEO: " 
	     << r.Timeout() << "]";
}

