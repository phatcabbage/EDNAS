#ifndef TCP__PACKET_HXX__
#define TCP__PACKET_HXX__

#include <cstring>
#include "Protocol.h++"
#include "Network/Packet.h++"

namespace TCP
{
    struct Protocol;
    struct Packet : public Network::IPacket
    {
	typedef std::shared_ptr<Packet> Ptr;

	static const SizeType HEADER_SIZE = 160 * Bandwidth::BIT;
	SeqNumT SEQ;
	SeqNumT ACK;
	int Flags;
	std::shared_ptr<IData> Data;

	Packet(int flags = Protocol::None,
	       SeqNumT seq = 0,
	       SeqNumT ack = 0,
	       int payloadType = Protocols::FAKE,
	       Protocol::AddressType receiver = Protocol::INVALID_ADDRESS,
	       Protocol::AddressType sender = Protocol::DEFAULT_ADDRESS,
	       SizeType size = 0)
	    : Network::IPacket
	      (Protocol::ID, payloadType, receiver, 
	       sender, size + HEADER_SIZE, false)
	    , SEQ(seq)
	    , ACK(ack)
	    , Flags(flags)
	    , Data()
	{}

	Packet(const Packet& p)
	    : Network::IPacket(Protocol::ID, p.PayloadType, p.Receiver,
			       p.Sender, p.Length, p.HasData)
	    , SEQ(p.SEQ)
	    , ACK(p.ACK)
	    , Flags(p.Flags)
	    , Data(p.Data)
	{}

	static 
	std::list<Ptr>
	Wrap(std::shared_ptr<IData> data)
	{
	    SizeType size = data->Size();
	    int numpackets = (size / TCP::Protocol::DEF_MAX_SEGMENT_SIZE) + 1;
	    std::list<Ptr> result(numpackets);
	    
	    std::list<Ptr>::reverse_iterator it =
		result.rbegin(), end = result.rend();
	    
	    it->reset
		(new Packet(Protocol::Data, 
			    0, 
			    0, 
			    Protocols::FAKE, 
			    0, 
			    0,
			    (size % TCP::Protocol::DEF_MAX_SEGMENT_SIZE)));
	    (*it)->HasData = true;
	    (*it)->Data = data;

	    while(++it != end)
	    {
		it->reset(new Packet(Protocol::Data,
				     0,
				     0,
				     Protocols::FAKE,
				     0,
				     0,
				     Protocol::DEF_MAX_SEGMENT_SIZE));
		(*it)->Data = data;
	    }

	    return result;
	}
    };
    typedef std::shared_ptr<Packet> PacketPtr;
}

#endif // TCP__PACKET_HXX__
