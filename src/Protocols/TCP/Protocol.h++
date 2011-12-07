#ifndef TCP__PROTOCOL_HXX__
#define TCP__PROTOCOL_HXX__

#include "SEDNAS.h++"
#include "Protocols/IPv4.h++"

#include "Protocols/ProtocolList.h++"

namespace TCP
{
    struct Node;
    struct Packet;
    struct Protocol;
    typedef TAddress<Protocol> Address;
    typedef Address Port;
    struct Socket;
    
    struct Protocol
    {
	typedef IPv4::Protocol WrapperType;
	static const Protocols::ProtocolID ID = Protocols::TCP;
	static const int Layer = IPv4::Protocol::Layer + 1;

	enum MessageType
	{
	    None		= 0,
	    Synchronize		= 1,
	    Acknowledge		= (Synchronize << 1),
	    Data		= (Acknowledge << 1),
	    Close		= (Data << 1)
	};

	enum ConnectionState
	{
	    Closed,
	    Listening,
	    Open,
	    Closing
	};

	typedef Packet PacketType;
	typedef std::shared_ptr<PacketType> PacketPtr;
	typedef Port AddressType;
	
	typedef int SequenceNumberType;

	static const SequenceNumberType	DEF_INITIAL_WINDOW_SIZE;
	static const SequenceNumberType	DEF_MAX_WINDOW_SIZE;
	static const SequenceNumberType	DEF_WINDOW_SCALE_FACTOR;
	static const SizeType	DEF_MAX_SEGMENT_SIZE;
	static const TimeType	DEF_TIMEOUT;
	static const int	DEF_MAX_TIMEOUTS;

	static const Port	MIN_UNPRIVILEGED_PORT;
	static const Port	MAX_PORT;

	static const AddressType DEFAULT_ADDRESS;
	static const AddressType INVALID_ADDRESS;
    };
    
    typedef Protocol::SequenceNumberType SeqNumT;

    struct Socket
    {
    	IPv4::Protocol::AddressType	IP;
    	TCP::Protocol::AddressType	Port;

    	bool 
    	operator<(const Socket& s) const
    	{ return ((IP < s.IP) || ((Port < s.Port) && (IP == s.IP))); }

    	bool 
    	operator!() const
    	{ return (!IP && !Port); }

    	Socket&
    	operator=(const Socket& s)
    	{ IP = s.IP; Port = s.Port; return *this;}
	
    	bool
    	operator==(const Socket& s) const
    	{ return (IP == s.IP && Port == s.Port); }
	
	Socket(IPv4::Protocol::AddressType ip =
	       IPv4::Protocol::INVALID_ADDRESS,
	       TCP::Protocol::AddressType port =
	       Protocol::INVALID_ADDRESS)
	    : IP(ip)
	    , Port(port)
	{}
    };
}

std::ostream&
operator<<(std::ostream&, const TCP::Port&);

std::ostream&
operator<<(std::ostream&, const TCP::Socket&);

#endif // TCP__PROTOCOL_HXX__
