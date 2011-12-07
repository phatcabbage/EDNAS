#include "SLAAP.h++"
#include "Tracker/Tracker.h++"
#include "Transfer/TransferQueue.h++"
#include "Tracker/PeerInfo.h++"
#include "Tracker/TrackerRequest.h++"
#include "Tracker/TrackerResponse.h++"
#include "Tracker/ContactList.h++"
#include "Transfer/Message.h++"
#include "Peer/Peer.h++"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <vector>


using std::random_shuffle;

namespace Tracker
{
    const Bandwidth Tracker::DEF_UPSTREAM = 10240;
    const Bandwidth Tracker::DEF_DOWNSTREAM = 10240;
    const Time Tracker::DEF_ANNOUNCE_INTERVAL = 60;
    const Time Tracker::DEF_MIN_ANNOUNCE_INTERVAL = 10;
    const uint Tracker::DEF_CONTACT_LIST_SIZE = 30;

    Tracker::
    Tracker() 
	: Transfer::Transferer(DEF_UPSTREAM, DEF_DOWNSTREAM),
	  _peers(),
	  _seeders(),
	  _leechers(),
	  _contactListSize(DEF_CONTACT_LIST_SIZE),
	  _announceInterval(DEF_ANNOUNCE_INTERVAL),
	  _minAnnounceInterval(DEF_MIN_ANNOUNCE_INTERVAL)
    {
	/* Left blank. */
    }

    void
    Tracker::
    processStartRequest(Transfer::Message<Peer::Peer,Tracker,TrackerRequest>* m)
    {
	Peer::Peer* p = m->sender;
	assert(_peers.count(p) == 0);
	assert(_seeders.count(p) == 0);
	assert(_leechers.count(p) == 0);
	
	PeerInfo *pi = new PeerInfo(*(m->payload));
	_peers[p] = pi;
	(pi->completed ? _seeders : _leechers).insert(p);

	sendResponse(p);
    }

    void
    Tracker::
    processUpdateRequest(Transfer::Message<Peer::Peer,Tracker,TrackerRequest>* m)
    {
	Peer::Peer* p = m->sender;
	assert(_peers.count(p) == 1);
	assert((_seeders.count(p) == 1) || (_leechers.count(p) == 1));

	PeerInfo* pi = _peers[p];
	pi->update(*(m->payload));

	sendResponse(p);
    }

    void
    Tracker::
    processStopRequest(Transfer::Message<Peer::Peer,Tracker,TrackerRequest>* m)
    {
	Peer::Peer* p = m->sender;
	assert(_peers.count(p) == 1);
	assert((_seeders.count(p) == 1) || (_leechers.count(p) == 1));

	PeerInfo* pi = _peers[p];
	_peers.erase(p);
	delete pi;

	_seeders.erase(p);
	_leechers.erase(p);
    }

    void
    Tracker::
    processCompleteRequest(Transfer::Message<Peer::Peer,Tracker,TrackerRequest>* m)
    {
	Peer::Peer* p = m->sender;
	processUpdateRequest(m);
	
	_leechers.erase(p);
	_seeders.erase(p);
	
	sendResponse(p);
    }

    void
    Tracker::
    sendResponse(Peer::Peer* p)
    {
	PeerInfo* pi = _peers[p];
	assert(pi);
	std::list<Peer::Peer*> list(_seeders.begin(), _seeders.end());
	list.insert(list.end(), _leechers.begin(), _leechers.end());
	ContactList* peers = new ContactList(list, p, pi->numwant);
	TrackerResponse* tr = 
	    new TrackerResponse(_announceInterval, _minAnnounceInterval,
				_seeders.size(), _leechers.size(), peers);
	Transfer::Message<Tracker,Peer::Peer,TrackerResponse>* m =
	    new Transfer::Message<Tracker,Peer::Peer,TrackerResponse>(this, p, tr);
	send(m, p);
    }

    void
    Tracker::
    send(Transfer::AbstractMessage* m, Peer::Peer* p)
    {
	Transfer::TransferQueue* tq = Transfer::TransferQueue::instance();
	tq->transfer(m, this, p);
    }
	

/**
 * This is an interface function that receives a TrackerRequestMessage.
 * It adds the peer to the tracker's list of known peers and sends
 * a TrackerResponseMessage to the peer, which contains a list of 
 * potential contacts.
 */
    void
    Tracker::
    receive(Transfer::Message<Peer::Peer,Tracker,TrackerRequest> *m)
    {
	TrackerRequest::Event e = m->payload->event;
	switch(e)
	{
	case (TrackerRequest::Update):
	    processUpdateRequest(m);
	    break;
	case (TrackerRequest::Start):
	    processStartRequest(m);
	    break;
	case (TrackerRequest::Stop):
	    processStopRequest(m);
	    break;
	case (TrackerRequest::Complete):
	    processCompleteRequest(m);
	    break;
	default:
	    break;
	}
	delete m;
    }	    
    
}
