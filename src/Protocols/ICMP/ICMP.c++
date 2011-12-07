#include <iostream>

#include "Protocol.h++"

namespace ICMP
{
    const Protocol::WrapperType::WrappedType
    Protocol::TypeCode;
    
    const SizeType Packet::HEADER_SIZE = 64 * Size::BYTE;
    
    static const char* SubTypeNames[] =
    { 
	"Echo Request",
	"Echo Reply",
	"Destination Unreachable",
	"Redirect",
	"Source Quench",
	"Time Exceeded"
    };
}
	
