#ifndef __ARCHITECTURES__BITTORRENT__TRACKER_HXX__
#define __ARCHITECTURES__BITTORRENT__TRACKER_HXX__

#include "SEDNAS.h++"
#include "Transfer/Transferer.h++"
#include "./BitTorrent.h++"
#include "./Interfaces/ITracker.h++"

#include <set>
#include <map>
#include <vector>

namespace Architectures
{
    namespace BitTorrent
    {
	class Tracker 
	    : public Transfer::Transferer,
	      public ITracker
	{
	private:
	    typedef
	    TrackerRequestMessage
	    RqMsg;

	    typedef 
	    std::set<Peer*> 
	    PeerSet;

	    enum 
	    {
		DEF_UPSTREAM = 1048576, 
		DEF_DOWNSTREAM = 1048576
	    };

	    TorrentFile* 
	    _f;

	    PeerSet 
	    _seeders;
	    
	    PeerSet 
	    _leechers;

	public:
	    Tracker(TorrentFile* f);

	Events:

	    Event
	    startRequest     (RqMsg* m){_leechers.insert(m->payload->peer);}

	    Event
	    updateRequest    (RqMsg* m){_leechers.insert(m->payload->peer);}

	    Event
	    stopRequest      (RqMsg* m){_leechers.remove(m->payload->peer);}

	    Event
	    completionRequest(RqMsg* m){_seeders.insert (m->payload->peer);}

	Actions:

	    Action
	    sendResponse(Peer* p);
	    
	Notifications:

	    Notification
	    receive(TrackerRequestMessage* m);
	};

	class TrackerRequest
	{
	public:
	    Peer* peer;
	    TrackerRequest(Peer* a_peer) : peer(a_peer) {}
	};

	class TrackerResponse
	{
	public:
	    const std::vector<Peer*> peers;
	    TrackerResponse(const std::vector<Peer*> a_peers) : peers(a_peers){}
	};
    }
}

#endif // __ARCHITECTURES__BITTORRENT__TRACKER_HXX__
