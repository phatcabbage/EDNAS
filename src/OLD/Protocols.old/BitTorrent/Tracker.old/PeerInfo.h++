#ifndef __TRACKER__PEER_INFO_HXX__
#define __TRACKER__PEER_INFO_HXX__

#include "SEDNAS.h++"

#include <string>

namespace Tracker
{
    class TrackerRequest;

    struct PeerInfo
    {
	std::string peer_id;
	Datasize uploaded;
	Datasize downloaded;
	Datasize left;
	const Time startTime;
	Time lastAnnounce;
	uint numwant;
	bool compact;
	bool completed;
	bool alive;
	
	PeerInfo(std::string& a_peer_id,
		 Datasize a_uploaded,
		 Datasize a_downloaded,
		 Datasize a_left,
		 uint a_numwant,
		 Time a_startTime,
		 bool a_compact);

	explicit
	PeerInfo(const TrackerRequest& tr);
	
	void update(const TrackerRequest& tr);
    };
}


#endif // __TRACKER__PEERINFO_HXX__
