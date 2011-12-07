#ifndef __ARCHITECTURES__BITTORRENT__INTERFACES__IPEER_HXX__
#define __ARCHITECTURES__BITTORRENT__INTERFACES__IPEER_HXX__

#include "SEDNAS.h++"
#include "../BitTorrent.h++"

namespace Transfer
{
    template<typename Sender,typename Receiver, typename Payload>
    class Message;
}

namespace Architectures
{
    namespace BitTorrent
    {
	class IPeer;
	class ITracker;
	class TrackerRequest;
	class TrackerResponse;

	class IPeer
	{
	public:
	    
	    virtual
	    void
	    receive(HandshakeMessage* m) = 0;

	    virtual
	    void
	    receive(ChokeMessage* m) = 0;

	    virtual
	    void
	    receive(UnchokeMessage* m) = 0;

	    virtual
	    void
	    receive(InterestedMessage* m) = 0;

	    virtual
	    void
	    receive(UninterestedMessage* m) = 0;

	    virtual
	    void
	    receive(HaveMessage* m) = 0;

	    virtual
	    void
	    receive(BitfieldMessage* m) = 0;

	    virtual
	    void
	    receive(RequestMessage* m) = 0;

	    virtual
	    void
	    receive(PieceMessage* m) = 0;

	    virtual
	    void
	    receive(CancelMessage* m) = 0;

	    virtual
	    void
	    receive(KeepAliveMessage* m) = 0;
	};
    } // BitTorrent
} // Architectures

#endif // __ARCHITECTURES__BITTORRENT__INTERFACES__IPEER_HXX__
