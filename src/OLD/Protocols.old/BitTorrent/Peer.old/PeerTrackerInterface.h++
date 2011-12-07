#ifndef __PEER__PEERTRACKERINTERFACE_HXX__
#define __PEER__PEERTRACKERINTERFACE_HXX__

#include "SEDNAS.h++"

namespace Tracker { class Tracker; class TrackerResponse;}
namespace Transfer{ template<> Message<Tracker::Tracker,Peer::Peer,Tracker::TrackerResponse>;}

namespace Peer
{
    class Peer;

    interface PeerTrackerInterface
    {
	virtual void receive(Transfer::Message<Tracker::Tracker,Peer,Tracker::TrackerResponse>* m) = 0;
    };
}

#endif // __PEER__PEERTRACKERINTERFACE_HXX__
