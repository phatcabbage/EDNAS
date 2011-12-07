#include <iomanip>
#include <sstream>
#include <string>

#include "Protocol.h++"

namespace Ethernet
{

    const char* SubTypeNames[] =
    {
	"IPv4",	"IPv6",	"ARP",	"RARP",	"Fake"
    };

    const AddressType DEFAULT	= 0x000000000000;
    const AddressType BROADCAST	= 0xFFFFFFFFFFFF;

    AddressType G_MAC = 0;
}

std::string
ToString(Protocols::Ethernet::SubType s)
{
    return std::string(EthernetSubtypeNames[s]);
}

std::ostream&
operator<<(std::ostream& o, const Protocols::Ethernet::AddressType& mac)
{
     o.flags(std::ios::hex);
     for(int i = 5; i > 0; --i)
	 o << std::setw(2) << std::setfill('0') << mac.Byte(i) << ":";
     return o << std::setw(2) << std::setfill('0') << mac.Byte(0);
}
