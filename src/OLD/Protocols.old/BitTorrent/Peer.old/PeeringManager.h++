#ifndef __PEER__PEERINGMANAGER_HXX__
#define __PEER__PEERINGMANAGER_HXX__

#include "../Timing.h++"

#include <set>
#include <map>
#include <vector>

namespace Peer
{
    class Peer;
    class PeeringManager;
    struct PeeringManagerTimerEntry;

    typedef Peer* PeerHandle;

    typedef void (PeeringManager::*PeeringManagerFunction)(TimerID);

    class PeeringManager
    {
	
	friend class PeeringManagerTimerEntry;
    private:

	// Constant defaults
	
	/**
	 * The default amount of time between scheduled updates.
	 */
	static const Time DEF_TIME_BETWEEN_UPDATES;

	/**
	 * The default amount of time to wait for a tracker to
	 * respond before we consider it unresponsive.
	 */
	static const Time DEF_TRACKER_RESPONSE_WAIT;

	/**
	 * The default amount of time to wait for a peer to respond
	 * to our handshake before we consider the peer unresponsive.
	 */
	static const Time DEF_HANDSHAKE_WAIT;

	/**
	 * The default minimum number of peers to know before
	 * we stop asking the tracker for more.
	 */
	static const uint DEF_MIN_KNOWN_PEERS;

	/**
	 * The peer with which this PeeringManager is associated.
	 */
	Peer& _peer;
    
	/**
	 * The peers that we have contacted and exchanged handshakes with.
	 */
	std::set<PeerHandle> _knownPeers;

	/**
	 * Peers that still haven't responded to our handshakes, but haven't timed
	 * out yet either.
	 */
	std::map<PeerHandle, TimerID> _unansweredHandshakes;

	Time _timeBetweenUpdates;
	Time _trackerResponseWait;
	Time _handshakeWait;
	uint _minKnownPeers;

	TimerID _updateTimer;
	TimerID _trackerRequestTimer;

    protected:

	TimerID scheduleTimer(PeeringManagerFunction f, Time delta);
	void cancelTimer(TimerID tid);

	void sendTrackerRequest(void);
	void sendHandshake(Peer* p);
	void sendHandshakeResponse(Peer* p);
	void sendBitfield(Peer* p);

	void trackerRequestTimeout(TimerID tid);
	void handshakeTimeout(TimerID tid);

	inline bool trackerResponsePending(void) const { return (_trackerRequestTimer == Timing::TimerEntry::INVALID_TIMER); }
	inline uint potentialPeers(void) const { return (_knownPeers.size() + _unansweredHandshakes.size()); }

	void processPeerCount(void);

    public:
	PeeringManager(Peer& p);
    
	void start(void);

	void process(void);

	void handshake(Peer* p);
	void trackerResponse(std::vector<Peer*>* cList);
    	void handshakeResponse(Peer* p);
    };

    struct PeeringManagerTimerEntry : public Timing::TimerEntry
    {
	PeeringManager* const pm;
	PeeringManagerFunction f;
	
	PeeringManagerTimerEntry(PeeringManager* p, PeeringManagerFunction func);
	virtual void execute(TimerID);
    };
}

#endif // __PEERINGMANAGER_HXX__
