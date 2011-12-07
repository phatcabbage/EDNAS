#ifndef TCP__RTTMANAGER_HXX__
#define TCP__RTTMANAGER_HXX__

#include <cstdio>
#include "SEDNAS.h++"
#include "../Protocol.h++"
#include "../ConnectionManager.h++"


namespace TCP
{
    struct RTTManager : public ConnectionManager
    {
	static const TimeType 
	INVALID_TIME = -1;

	// RFC 2988 Equation 2.1
	static const TimeType 
	DEFAULT_INITIAL_TIMEOUT = 3 * Time::SECOND;

	// RFC 2988 Equation 2.2
	static const int 
	DEFAULT_LOG_INV_ALPHA = 3;

	// RFC 2988 Equation 2.2
	static const int 
	DEFAULT_LOG_INV_BETA = 2;

	// RFC 2988 Equation 2.{2,3}
	static const int 
	DEFAULT_LOG_K = 2; 

	const 
	int
	ID;

	virtual void
	Acknowledged(SeqNumT ack);
	
	virtual void
	TimedOut(SeqNumT seq);

	RTTManager(Connection& c,
		   int a = DEFAULT_LOG_INV_ALPHA,
		   int b = DEFAULT_LOG_INV_BETA,
		   int k = DEFAULT_LOG_K);

	~RTTManager();

    protected:
	static 
	int 
	G_ID;

	FILE*
	Log;

	int
	Alpha_;
	
	int
	Beta_;

	int
	K_;

	TimeType 
	Smoothed_;
	
	TimeType
	Variance_;

	TimeType
	Timeout_;
	
	void
	Acknowledged(TimeType rtt);

	void
	TimedOut();

	// Accessors
    public:
	int 
	Alpha() const
	{ return Alpha_; }

	int 
	Beta() const
	{ return Beta_; }

	int 
	K() const
	{ return K_; }

	TimeType
	SmoothedRTT() const
	{ return Smoothed_; }

	TimeType
	Variance() const
	{ return Variance_; }

	TimeType
	Timeout() const
	{ return Timeout_; }

	void 
	Alpha(int i);

	void 
	Beta(int i);

	void 
	K(int i);
    };
}

std::ostream&
operator<<(std::ostream&,const TCP::RTTManager&);

#endif // TCP__RTTMANAGER_HXX__
