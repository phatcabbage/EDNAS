#include "ResultsManager.h++"
#include "Simulation/Timer.h++"
#include "../Connection.h++"

namespace TCP
{
    static const char* STR_CREATED	= "CREATED ";
    static const char* STR_SENT		= "SENT    ";
    static const char* STR_ACK		= "ACK     ";
    static const char* STR_DUPACK	= "DUPACK  ";
    static const char* STR_TIMEDOUT	= "TIMEOUT ";

    int
    ResultsManager::G_ID;

    static
    std::string
    CreateNameFromID(Connection& c)
    {
	std::ostringstream oss;
	oss << "TCP(" << c.LocalSocket() << "->" << c.RemoteSocket() << ").log";
	return oss.str();
    }

    ResultsManager::
    ResultsManager(Connection& c)
	: ConnectionManager(c)
	, ID(++G_ID)
	, Log()
    {
	std::string fname = CreateNameFromID(c);
	Log = fopen(fname.c_str(), "w");
	if(!Log)
	{
	    fprintf(stderr, "Unable to open %s for writing. Exiting.\n", fname.c_str());
	    exit(1);
	}
    }

    ResultsManager::
    ~ResultsManager()
    {
	fclose(this->Log);
    }
    
    void
    ResultsManager::
    Created(SeqNumT seq)
    {
	Record(seq, STR_CREATED);
    }

    void
    ResultsManager::
    Sent(SeqNumT seq)
    {
	Record(seq, STR_SENT);
    }

    void
    ResultsManager::
    Acknowledged(SeqNumT ack)
    {
	SeqNumT seq = ack - 1;
	Record(seq, STR_ACK);
    }

    void
    ResultsManager::
    DuplicateAcknowledged(SeqNumT ack)
    {
	SeqNumT seq = ack - 1;
	Record(seq, STR_DUPACK);
    }

    void
    ResultsManager::
    TimedOut(SeqNumT seq)
    {
	Record(seq, STR_TIMEDOUT);
    }

    void
    ResultsManager::
    RTT(SeqNumT seq, TimeType time)
    {
	// Log << TimeToString(time) << "\t" 
	//     << "RTT" << "\t" 
	//     << seq   << "\t" 
	//     << time 
	//     << Logging::endl;
    }	

    template<int n>
    struct PrintfFmter
    { static const char* FmtString; };

    template<>
    const char*
    PrintfFmter<4>::FmtString = "%20lld\t%s\t%10d\n";
	
    template<>
    const char*
    PrintfFmter<8>::FmtString = "%20ld\t%s\t%10d\n";

    void
    ResultsManager::
    Record(SeqNumT seq, const char* event)
    {
	static const char *fmtString = PrintfFmter<sizeof(long)>::FmtString;
	fprintf(this->Log, fmtString,
		 Timer::Main.Time(), event, seq);
    }
}
