#ifndef TCP__CONNECTION__BASE_HXX__
#define TCP__CONNECTION__BASE_HXX__

#include "SEDNAS.h++"
#include "Network/Packet.h++"
#include "Protocols/TCP/Protocol.h++"

namespace TCP
{
    struct Connection;
    namespace _Connection
    {
	struct Base
	{
	    typedef std::shared_ptr<Network::IData> DataPtr;
	    
	    struct Message
	    {
		Connection& 
		Conn;

		DataPtr 
		Data;

		Message(Connection& c, DataPtr d) 
		    : Conn(c), Data(d) {}
	    };
	    
	    struct WindowStatus
	    {
		SeqNumT
		SEQ;	// Sequence Number

		SeqNumT
		ACK;	// Acknowledge Number

		SeqNumT
		RWND;	// Receive Window
	    };
	    
	    static const SeqNumT
	    DEF_LOCAL_RCV_WND = 64;
	    
	    static const SeqNumT
	    DEF_REMOTE_RCV_WND = DEF_LOCAL_RCV_WND;
	    
	    static const int 
	    DEF_MAX_TIMEOUTS = 30;
	};
    }
}

#endif // TCP__CONNECTION__BASE_HXX__
