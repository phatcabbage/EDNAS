#ifndef NETWORK__FRAME_HXX__
#define NETWORK__FRAME_HXX__

#include "./Ethernet.h++"

// #include <iosfwd>

// namespace Network
// {
//     typedef unsigned int MACAddress;
//     struct Frame
//     {
// 	enum PayloadType
// 	{
// 	    FAKE,
// 	    IPv4,
// 	    IPv6,
// 	    ARP,
// 	    RARP
// 	};

// 	enum { HEADER_SIZE = 176, FOOTER_SIZE = 128 };

// 	MACAddress
// 	Sender;
	
// 	MACAddress
// 	Receiver;
	
// 	double
// 	Size;
	
// 	PayloadType
// 	Type;
	
// 	Frame(MACAddress sender,
// 	      MACAddress receiver,
// 	      double datasize,
// 	      PayloadType type)
// 	    : Sender(sender)
// 	    , Receiver(receiver)
// 	    , Size(HEADER_SIZE + FOOTER_SIZE + datasize)
// 	    , Type(type)
// 	{}
// 	virtual ~Frame(){}
//     };

//     inline std::ostream&
//     operator<<(std::ostream& o, const Frame& f);
// }
#endif // NETWORK__FRAME_HXX__
