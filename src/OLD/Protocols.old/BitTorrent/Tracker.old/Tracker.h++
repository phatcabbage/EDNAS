#ifndef __TRACKER__TRACKER_HXX_
#define __TRACKER__TRACKER_HXX_

#include "SEDNAS.h++"
#include "Transfer/Transferer.h++"
#include "Transfer/Message.h++"
#include "Peer/Peer.h++"

#include <list>
#include <map>
#include <set>
#include <vector>

namespace Transfer { class AbstractMessage; }

/**
 * The Tracker class plays the role of (you guessed it) the tracker in
 * a BitTorrent network. It waits for connections from peers and then 
 * sends them a list of contacts.
 */
namespace Tracker
{
    class ContactList;
    class PeerInfo;
    class TrackerRequest;

    const unsigned int DEFAULT_CONTACT_LIST_SIZE = 10;

    class Tracker
	: public Transfer::Transferer
    {
    private:
	typedef std::set<Peer::Peer*> Peers;
	typedef std::map<Peer::Peer*,PeerInfo*> PeerMap;
	
	static const Bandwidth DEF_UPSTREAM;
	static const Bandwidth DEF_DOWNSTREAM;
	static const Time DEF_ANNOUNCE_INTERVAL;
	static const Time DEF_MIN_ANNOUNCE_INTERVAL;
	static const uint DEF_CONTACT_LIST_SIZE;

	PeerMap _peers;
	Peers _seeders;
	Peers _leechers;

	uint _contactListSize;

	Time _announceInterval;
	Time _minAnnounceInterval;

	void processStartRequest   (Transfer::Message<Peer::Peer,Tracker,TrackerRequest>* m);
	void processUpdateRequest  (Transfer::Message<Peer::Peer,Tracker,TrackerRequest>* m);
	void processStopRequest    (Transfer::Message<Peer::Peer,Tracker,TrackerRequest>* m);
	void processCompleteRequest(Transfer::Message<Peer::Peer,Tracker,TrackerRequest>* m);

	void sendResponse(Peer::Peer* p);
	
	void send(Transfer::AbstractMessage* m, Peer::Peer* p);

    public:
	
	/** Creates an instance of Tracker. */
	Tracker();
	
	/** This method is called whenever a TrackerRequest message is sent to the Tracker.
	 *  (Usually by a Peer). The Tracker will respond with a TrackerResponse message that
	 *  includes a list of contacts.
	 */
	void receive(Transfer::Message<Peer::Peer,Tracker,TrackerRequest>* m);


	// /** Sets the maximum size of the list of contacts sent back to peers. */
	// inline void setContactListSize(const int listSize){_contactListSize = listSize;}
	// /** Gets the maximum size of the list of contacts sent back to peers. */
	// inline uint getContactListSize(void) { return _contactListSize;}
    };
}

#endif // __TRACKER__TRACKER_HXX__
