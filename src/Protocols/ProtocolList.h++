#ifndef PROTOCOLS__PROTOCOLLIST_HXX__
#define PROTOCOLS__PROTOCOLLIST_HXX__

namespace Protocols
{
    enum ProtocolID
    {
	FAKE,		ARP,		DHCP,
	ETHERNET,	ICMP,		IPv4,
	TCP,		TCPECHO,	TFTP,
	UDP,		UDPECHO
    };

    // static const char* ProtocolName[] =
    // {
    // 	"Fake",		"ARP",		"DHCP",
    // 	"Ethernet",	"ICMP",		"IPv4",
    // 	"TCP",		"TCP Echo",	"TFTP",
    // 	"UDP",		"UDP Echo",	NULL
    // };
    
}

#endif // PROTOCOLS__PROTOCOLLIST_HXX__
	
	
