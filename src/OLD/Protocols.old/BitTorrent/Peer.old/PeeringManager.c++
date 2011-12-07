#include "../SLAAP.h++"
#include "./Peer/PeeringManager.h++"
#include "../Peer.h++"
#include "../Tracker.h++"
#include "../Timing.h++"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <list>

namespace Peer
{
    const uint PeeringManager::DEF_MIN_KNOWN_PEERS = 10;
    const Time PeeringManager::DEF_HANDSHAKE_WAIT = 5;
    const Time PeeringManager::DEF_TRACKER_RESPONSE_WAIT = 5;
    const Time PeeringManager::DEF_TIME_BETWEEN_UPDATES = 10;

    PeeringManager::
    PeeringManager(Peer& p)
	: _peer(p),
	  _knownPeers(),
	  _unansweredHandshakes(),
	  _timeBetweenUpdates(DEF_TIME_BETWEEN_UPDATES),
	  _trackerResponseWait(DEF_TRACKER_RESPONSE_WAIT),
	  _handshakeWait(DEF_HANDSHAKE_WAIT),
	  _minKnownPeers(DEF_MIN_KNOWN_PEERS),
	  _updateTimer(Timing::TimerEntry::INVALID_TIMER),
	  _trackerRequestTimer(Timing::TimerEntry::INVALID_TIMER)	  
	  
    {
	/* Left blank. */
    }

    TimerID
    PeeringManager::
    scheduleTimer(PeeringManagerFunction f, Time delta)
    {
	Time t = Timing::Clock::instance()->time() + delta;
	PeeringManagerTimerEntry *te = new PeeringManagerTimerEntry(this, f);
	return Timing::Timer::instance()->create(t,te);
    }

    void
    PeeringManager::
    cancelTimer(TimerID tid)
    {
	Timing::Timer::instance()->cancel(tid);
    }

    void
    PeeringManager::
    sendTrackerRequest(void)
    {
	_peer.sendTrackerRequest();
	_trackerRequestTimer = scheduleTimer(&PeeringManager::trackerRequestTimeout, _trackerResponseWait);
    }

    void
    PeeringManager::
    sendHandshake(Peer* p)
    {
	_peer.sendHandshake(p);
	_unansweredHandshakes[p] = scheduleTimer(&PeeringManager::handshakeTimeout, _handshakeWait);
    }
    
    void
    PeeringManager::
    sendHandshakeResponse(Peer* p)
    {
	_peer.sendHandshakeResponse(p);
    }

    void
    PeeringManager::
    sendBitfield(Peer* p)
    {
	_peer.sendBitfield(p);
    }
    
    void
    PeeringManager::
    trackerRequestTimeout(TimerID tid)
    {
	printf("[PPM]\tError! Tracker response timeout for Peer %u!\n", _peer.id);
	_trackerRequestTimer = Timing::TimerEntry::INVALID_TIMER;

	processPeerCount();
    }


    void
    PeeringManager::
    handshakeTimeout(TimerID tid)
    {
	printf("[PPM]\tPeer %u had a handshake time out.\n", _peer.id);

	std::map<PeerHandle, TimerID>::iterator it;
	for (it = _unansweredHandshakes.begin(); it != _unansweredHandshakes.end(); it++)
	{
	    if (it->second == tid)
	    {
		_unansweredHandshakes.erase(it);
		break;
	    }
	}

	processPeerCount();
    }	

// inlined
/*
    bool
    PeeringManager::
    trackerResponsePending(void) const
    {
	return (_trackerResponseTimer == Timing::TimerEntry::INVALID_HANDLE);
    }

    uint
    PeeringManager::
    potentialPeers(void) const
    {
	return (_knownPeers.size() + _unansweredHandshakes.size());
    }

    PeeringManager::
    start(void)
    {
	sendTrackerRequest();
    }
*/

    void
    PeeringManager::
    processPeerCount(void)
    {
	bool needMorePeers = (potentialPeers() < _minKnownPeers);
	if (needMorePeers && !trackerResponsePending())
	    sendTrackerRequest();
    }

    void
    PeeringManager::
    process(void)
    {
	processPeerCount();
    }
    
    void
    PeeringManager::
    trackerResponse(std::vector<Peer*>* cList)
    {
	std::list<Peer*> newPeers;
	std::back_insert_iterator<std::list<Peer*> > newPeerIt(newPeers);

	cancelTimer(_trackerRequestTimer);

	std::sort(cList->begin(), cList->end());
	std::set_difference(cList->begin(), cList->end(),
			    _knownPeers.begin(), _knownPeers.end(), newPeerIt);

	foreach(Peer* p, newPeers)
	{
	    sendHandshake(p);
	}
    }

    void
    PeeringManager::
    handshake(Peer* p)
    {
	sendHandshakeResponse(p);
	sendBitfield(p);
	_knownPeers.insert(p);
    }

    void
    PeeringManager::
    handshakeResponse(Peer* p)
    {
	assert(_unansweredHandshakes.count(p) == 1);

	_unansweredHandshakes.erase(p);
	_knownPeers.insert(p);
	sendBitfield(p);

	processPeerCount();
}
}
namespace Peer
{
    PeeringManagerTimerEntry::
    PeeringManagerTimerEntry(PeeringManager* p, PeeringManagerFunction func)
	: pm(p),
	  f(func)
    {
	/* Left blank. */
    }

    void
    PeeringManagerTimerEntry::
    execute(TimerID tid)
    {
	(pm->*f)(tid);
    }
}
