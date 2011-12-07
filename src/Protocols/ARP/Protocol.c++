#include "Protocol.h++"

namespace ARP
{
    const int Protocol::Layer;

    const Protocols::ProtocolID Protocol::ID;

    const Protocol::AddressType Protocol::DEFAULT_ADDRESS = 
	IPv4::Protocol::DEFAULT_ADDRESS;

    const Protocol::AddressType Protocol::INVALID_ADDRESS = 
	IPv4::Protocol::INVALID_ADDRESS;

    const TimeType
    Protocol::
    TIMEOUT = 5 * Time::SECOND;

    const int
    Protocol::
    TIMEOUTS = 5;
}
