#include "Protocol.h++"
#include "Packet.h++"
#include "Protocols/IPv4/Packet.h++"
#include "Protocols/Ethernet/Packet.h++"

namespace TCP
{
    const Protocols::ProtocolID Protocol::ID;
    const int Protocol::Layer;
    const Protocol::SequenceNumberType Protocol::DEF_INITIAL_WINDOW_SIZE = 1;
    const Protocol::SequenceNumberType Protocol::DEF_MAX_WINDOW_SIZE = 64;
    const Protocol::SequenceNumberType Protocol::DEF_WINDOW_SCALE_FACTOR = 1;
    const SizeType Protocol::DEF_MAX_SEGMENT_SIZE = 
	(1500 * Size::BYTE) - 
	(Packet::HEADER_SIZE + IPv4::Packet::HEADER_SIZE + Ethernet::Packet::HEADER_SIZE);
    const TimeType Protocol::DEF_TIMEOUT = 10 * Time::SECOND;
    const int Protocol::DEF_MAX_TIMEOUTS = 4;
    const Port Protocol::MIN_UNPRIVILEGED_PORT = 1025;
    const Port Protocol::MAX_PORT = 65536;
    const Protocol::AddressType Protocol::DEFAULT_ADDRESS = 0;
    const Protocol::AddressType Protocol::INVALID_ADDRESS = -1;
}

std::ostream&
operator<<(std::ostream& o, const TCP::Port& port)
{ return o << port.Value; }

std::ostream&
operator<<(std::ostream& o, const TCP::Socket& sock)
{ return o << sock.IP << ":" << sock.Port; }
