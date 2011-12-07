#ifndef __ARCHITECTURES__BITTORRENT__PEER_HXX__
#define __ARCHITECTURES__BITTORRENT__PEER_HXX__

#include "SEDNAS.h++"
#include "./BitTorrent.h++"

namespace Architectures
{
    namespace BitTorrent
    {
	class Peer
	{

	    
	    
	Notifications:

	    /* Tracker */
	    Notification
	    receive(TrackerResponseMessage *m);

	    /* P2P */
	    Notification
	    receive(HandshakeMessage* m);

	    Notification
	    receive(ChokeMessage* m);
	    
	    Notification
	    receive(UnchokeMessage* m);

	    Notification
	    receive(InterestedMessage* m);

	    Notification
	    receive(UninterestedMessage* m);
	    
	    Notification
	    receive(HaveMessage* m);
	    
	    Notification
	    receive(BitfieldMessage* m);

	    Notification
	    receive(RequestMessage* m);

	    Notification
	    receive(PieceMessage* m);

	    Notification
	    receive(CancelMessage* m);

	    Notification
	    receive(KeepAliveMessage* m);
	};
    }
}


#endif // __ARCHITECTURES__BITTORRENT__PEER_HXX__
