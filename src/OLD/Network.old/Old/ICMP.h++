#ifndef ICMP_HXX__
#define ICMP_HXX__

struct IP;

template<typename _P>
struct Protocol
{
    enum { HEADER_SIZE = _P::HEADER_SIZE};
    struct Message;
};

struct ICMP
{
    enum { HEADER_SIZE = 512 }; // bytes (based on Linux-sized ping
    typedef IP ContainerProtocol;
    enum MessageType
    {
	Echo,
	EchoReply
    };
};

template<>
struct Protocol<ICMP>::Message : public IMessage
{
    const DataType

#endif // ICMP_HXX__
