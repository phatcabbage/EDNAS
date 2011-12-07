#ifndef ARP__PACKET_HXX__
#define ARP__PACKET_HXX__

#include "Protocol.h++"
#include "Network/Packet.h++"

namespace ARP
{
    struct Packet : public Network::IPacket
    {
	const static SizeType
	HEADER_SIZE = 208 * Bandwidth::BIT;
	
	MACAddress ReceiverMAC;
	MACAddress SenderMAC;

	static
	Network::PacketListPtr
	CreateReply(const Packet& request,
		    MACAddress replierMAC)
	{
	    Network::PacketListPtr pktList =
		std::make_shared<Network::PacketList>();

	    (*pktList)[Protocol::Layer] = 
		std::make_shared<Packet>
		(Protocol::Reply, request.Sender,
		 request.SenderMAC,
		 request.Receiver,
		 replierMAC);
	    return pktList;
	}

	static
	Network::PacketListPtr
	CreateRequest(IPAddress receiver,
		      IPAddress sender,
		      MACAddress senderMAC)
	{
	    Network::PacketListPtr pktList = 
		std::make_shared<Network::PacketList>();

	    (*pktList)[Protocol::Layer] = 
		std::make_shared<Packet>
		(Protocol::Request, receiver, 
		 Ethernet::Protocol::DEFAULT_ADDRESS,
		 sender, senderMAC);
	    return pktList;
	}

	Packet(Protocol::Message type,
	       IPAddress receiverIP,
	       MACAddress receiverMAC,
	       IPAddress senderIP,
	       MACAddress senderMAC)
	    : Network::IPacket(Protocol::ID, type, receiverIP, senderIP,
			       HEADER_SIZE, true)
	    , ReceiverMAC(receiverMAC)
	    , SenderMAC(senderMAC)
	{
	}
    };
}

#endif // ARP__PACKET_HXX__
