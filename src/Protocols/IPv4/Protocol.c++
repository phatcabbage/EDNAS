#include <cstdio>
#include <iostream>
#include "Protocol.h++"
#include "Protocols/ARP/Protocol.h++"

namespace IPv4
{
    const Protocols::ProtocolID Protocol::ID;
    const int Protocol::Layer;
    const IPv4::Protocol::AddressType IPv4::Protocol::BROADCAST_ADDRESS	= 0xFFFFFFFF; /* 255.255.255.255 */
    const IPv4::Protocol::AddressType IPv4::Protocol::LOCALHOST_ADDRESS	= 0x7F000001; /* 127.  0.  0.  1 */
    const IPv4::Protocol::AddressType IPv4::Protocol::DEFAULT_ADDRESS	= 0x00000000;  /*   0.  0.  0.  0 */
    const IPv4::Protocol::AddressType IPv4::Protocol::INVALID_ADDRESS	= 0x00000000;
    
    // Network class netmasks
    const IPv4::Protocol::AddressType IPv4::Protocol::CLASS_C	= 0xFFFFFF00; /* 255.255.255.  0 */
    const IPv4::Protocol::AddressType IPv4::Protocol::CLASS_B	= 0xFFFF0000; /* 255.255.  0.  0 */
    const IPv4::Protocol::AddressType IPv4::Protocol::CLASS_A	= 0xFF000000; /* 255.  0.  0.  0 */

    // static const char* SubTypeNames[] = 
    // {
    // 	"ICMP",
    // 	"IGMP",
    // 	"TCP",
    // 	"UDP",
    // 	"OSPF",
    // 	"SCTP"
    // };

    // std::string
    // Protocol::SubTypeName(Protocol::WrappedType w)
    // { return std::string(SubTypeNames[w]); }

    Logging::OStreamLogger
    Protocol::Log("IPv4");
}


std::ostream&
operator<<(std::ostream& o, const IPv4::Protocol::AddressType& ip)
{
    int p = ip;
    char buf[16]; /* "xxx.xxx.xxx.xxx\0" */
    snprintf(buf,16, "%d.%d.%d.%d", 
	     ((p >> 24) & 0xFF),
	     ((p >> 16) & 0xFF),
	     ((p >>  8) & 0xFF),
	     (p & 0xFF));
    return o << buf;
}

// IPv4::Address
// StringToAddress(const std::string& s)
// {
//     std::vector
