#ifndef PROTOCOLS__NODE_HXX__
#define PROTOCOLS__NODE_HXX__

#include <map>
#include <utility> // make_pair

#include "Simulation/Event.h++"
#include "Network/Packet.h++"

namespace Protocols
{
    namespace Templates
    {
	typedef Network::PacketListPtr Pkt;

	struct Sender
	{
	    virtual void
	    Send(Pkt& pkt, int receiver) = 0;
	    
	    virtual void
	    Send(Pkt& pkt, int receiver, int sender) = 0;
	};

	struct Demuxer
	{
	    virtual Event<Pkt&>&
	    operator[](int id) = 0;
	};
    }
}

#endif // PROTOCOLS__NODE_HXX__
