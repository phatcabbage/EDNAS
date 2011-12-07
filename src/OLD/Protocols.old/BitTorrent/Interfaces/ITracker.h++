#ifndef __ARCHITECTURES__BITTORRENT__INTERFACES__ITRACKER_HXX__
#define __ARCHITECTURES__BITTORRENT__INTERFACES__ITRACKER_HXX__

#include "SEDNAS.h++"

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

	typedef
	Transfer::Message<IPeer,ITracker,TrackerRequest>
	TrackerRequestMessage;

	typedef
	Transfer::Message<ITracker,IPeer,TrackerResponse>
	TrackerResponseMessage;


	class ITracker
	{
	public:
	    virtual 
	    void
	    receive(TrackerResponseMessage* m) = 0;
	};
    } // BitTorrent
} // Architectures


#endif //  __ARCHITECTURES__BITTORRENT__INTERFACES__ITRACKER_HXX__
