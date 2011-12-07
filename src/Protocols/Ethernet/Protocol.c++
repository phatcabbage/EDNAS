#include <iomanip>
#include <sstream>
#include <string>

#include "Protocols/Ethernet/Protocol.h++"

namespace Ethernet
{
    // static const char* SubTypeNames[] =
    // {
    // 	"IPv4",	"IPv6",	"ARP",	"RARP",	"Fake"
    // };

    const int			Protocol::Layer;//			= 0;
    const Protocols::ProtocolID	Protocol::ID;
    const Protocol::AddressType Protocol::INVALID_ADDRESS	=  0;
    const Protocol::AddressType Protocol::DEFAULT_ADDRESS	=  0;
    const Protocol::AddressType Protocol::BROADCAST_ADDRESS	= -1;

    Protocol::AddressType Protocol::G_MAC = 0;

    // std::string
    // Protocol::SubTypeName(Protocol::WrappedType w)
    // { return std::string(SubTypeNames[w]); }

    Logging::OStreamLogger 
    Protocol::Log("Ethernet");
}

std::ostream&
operator<<(std::ostream& o, const Ethernet::Protocol::AddressType& mac)
{
    o << "00:00:";
    o.flags(std::ios::hex);
    for(int i = 3; i > 0; --i)
	o << std::setw(2) << std::setfill('0') << mac.Byte(i) << ":";
    return o << std::setw(2) << std::setfill('0') << mac.Byte(0);
}
