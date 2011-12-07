#ifndef PROTOCOLS__TCP_HXX__
#define PROTOCOLS__TCP_HXX__

#include "SEDNAS.h++"
#include "./IPv4.h++"

namespace Protocols
{
    struct TCP
    {
	typedef IPv4 ContainerProtocol;
	static const ContainerProtocol::SubType
	TypeCode = IPv4::TCP;

	enum SubType
	{
	    ECHO,
	    EMPTY,
	    FTP,
	    FAKE,
	};

	enum PacketFlags : uint8_t
	{
	    CWR	= 1,
	    ECE	= 2,
	    URG	= 4,
	    ACK	= 8,
	    PSH	= 16,
	    RST	= 32,
	    SYN	= 64,
	    FIN	= 128
	};

	enum ConnectionState
	{
	    Closed,
	    Listening,
	    SynSent,
	    SynReceived,
	    Established,
	    FinWait1,
	    FinWait2,
	    CloseWait,
	    Closing,
	    LastAck,
	};

	struct Packet;
	typedef Packet PacketType;
	typedef std::shared_ptr<PacketType> PacketPtr;
	typedef TAddress<TCP> Port;
	typedef Port AddressType;
	struct Socket;
	
	typedef uint32_t SequenceNumber;

	static const Port	MIN_UNPRIVILEGED_PORT;
	static const Port	MAX_PORT;
	
	static const SizeType	DEFAULT_WINDOW		= 64 * Size::KILOBYTE;
    };
    
    struct TCP::Socket
    {
	IPv4::AddressType	IP;
	TCP::AddressType	Port;

	bool 
	operator<(const Socket& s) const
	{ 
	    return ((IP < s.IP) || ((Port < s.Port) && (IP == s.IP)));
	}

	bool 
	operator!() const
	{ return (!IP && !Port); }

	Socket&
	operator=(const Socket& s)
	{ IP = s.IP; Port = s.Port; return *this;}
	
	bool
	operator==(const Socket& s) const
	{ return (IP == s.IP && Port == s.Port); }
    };

    struct TCP::Packet
    {
	static const SizeType
	HEADER_SIZE = 160 * Bandwidth::BIT;

	TCP::SequenceNumber	SEQ;
	TCP::SequenceNumber	ACK;
	uint8_t			Flags;
	TCP::SubType		Type;
	std::shared_ptr<void>	Payload;
	SizeType		Length;
	SizeType		Window;
	SizeType		Data;
	TCP::Port		Receiver;
	TCP::Port		Sender;
	
	SizeType Size() const 
	{ return this->Length + HEADER_SIZE; }

	SizeType DataBytes() const 
	{ 
	    SizeType size = (this->Length / Size::BYTE) ;
	    if (Flags & TCP::SYN) ++size;
	    return size;
	}

	static Packet*
	SYNPacket(TCP::SequenceNumber seq)
	{
	    auto pkt = new Packet(seq);
	    pkt->Flags = TCP::SYN;
	    return pkt;
	}

	static Packet*
	SYNACKPacket(TCP::SequenceNumber seq,
		     TCP::SequenceNumber ack)
	{
	    auto pkt = new Packet(seq,ack);
	    pkt->Flags = (TCP::SYN + TCP::ACK);
	    return pkt;
	}

	Packet(TCP::SequenceNumber seq		= TCP::SequenceNumber(),
	       TCP::SequenceNumber ack		= TCP::SequenceNumber(),
	       TCP::SubType type		= TCP::EMPTY,
	       std::shared_ptr<void> payload	= std::shared_ptr<void>(),
	       SizeType size			= SizeType(0),
	       SizeType window			= TCP::DEFAULT_WINDOW,
	       TCP::Port receiver		= TCP::Port(),
	       TCP::Port sender 		= TCP::Port())
	    : SEQ(seq)
	    , ACK(ack)
	    , Flags()
	    , Type(type)
	    , Payload(payload)
	    , Length(size)
	    , Window(window)
	    , Receiver(receiver)
	    , Sender(sender)
	{}

	virtual ~Packet(){}
    };
}

std::ostream&
operator<<(std::ostream&, const Protocols::TCP::Port&);

std::ostream&
operator<<(std::ostream&, const Protocols::TCP::Socket&);

#endif // PROTOCOLS__TCP_HXX__
