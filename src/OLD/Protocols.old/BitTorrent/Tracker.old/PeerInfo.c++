#include "Tracker/PeerInfo.h++"
#include "Timing/Clock.h++"
#include "Tracker/TrackerRequest.h++"

#include <cassert>

namespace Tracker
{
    PeerInfo::
    PeerInfo(std::string& a_peer_id,
	     Datasize a_uploaded,
	     Datasize a_downloaded,
	     Datasize a_left,
	     uint a_numwant,
	     Time a_startTime,
	     bool a_compact)
	: peer_id(a_peer_id),
	  uploaded(a_uploaded),
	  downloaded(a_downloaded),
	  left(a_left),
	  startTime(a_startTime),
	  lastAnnounce(a_startTime),
	  numwant(a_numwant),
	  compact(a_compact),
	  completed(a_left == 0),
	  alive(true)
    {
	/* Left blank. */
    }

    PeerInfo::
    PeerInfo(const TrackerRequest& tr)
	: peer_id(tr.peer_id),
	  uploaded(tr.uploaded),
	  downloaded(tr.downloaded),
	  left(tr.left),
	  startTime(Timing::Clock::now()),
	  lastAnnounce(startTime),
	  numwant(tr.numwant),
	  compact(tr.compact),
	  completed(tr.left == 0),
	  alive(tr.event != TrackerRequest::Stop)
    {
	/* Left blank. */
    }
	  
    void
    PeerInfo::
    update(const TrackerRequest& tr)
    {
	Time now = Timing::Clock::now();

	assert(tr.peer_id == peer_id);
	assert(tr.uploaded >= uploaded);
	assert(tr.downloaded >= downloaded);
	assert(tr.left <= left);

	uploaded = tr.uploaded;
	downloaded = tr.downloaded;
	left = tr.left;
	lastAnnounce = now;
	numwant = tr.numwant;
	completed = (left == 0);
	alive = (tr.event == TrackerRequest::Stop);
    }
}
