#ifndef __TRACKERREQUEST_HXX__
#define __TRACKERREQUEST_HXX__

#include "SEDNAS.h++"
#include "Transfer/Message.h++"

#include <string>

namespace Tracker
{
    class Tracker;

    class TrackerRequest
    {
    public:
	enum Event
	{
	    Start,
	    Update,
	    Stop,
	    Complete
	};
	

	TrackerRequest(Datasize a_uploaded,
		       Datasize a_downloaded,
		       Datasize a_left,
		       Event a_event = Update,
		       bool a_compact = true,
		       bool a_no_peer_id = true,
		       uint a_numwant = 0);

	const uint info_hash; /* Unused */
	const std::string peer_id; /* Unused */ 
	const unsigned short port; /* Unused */
	const Datasize uploaded;
	const Datasize downloaded;
	const Datasize left;
	const Event event;
	const bool compact;
	const bool no_peer_id;
	const uint numwant;

	
	Datasize dataSize(void) const;
	
	std::string description(void) const;
	std::string longDescription(void) const;
    };
}

#endif 
	
