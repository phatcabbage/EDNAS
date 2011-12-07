#include "Tracker/TrackerResponse.h++"
#include "Tracker/ContactList.h++"
#include "Peer/Peer.h++"

#include <cstdio>

namespace Tracker
{
    Data
    TrackerResponse::
    dataSize() const
    {
	Data size = 0;
	size += 10; // MIME header, "interval", etc
	size += 8;  // seeders and leechers
	size += peers->dataSize;

	return size;
    }

    TrackerResponse::
    TrackerResponse(const Time&        a_interval,
		    const Time&        a_min_interval,
		          uint         a_complete,
		          uint         a_incomplete,
		          ContactList* a_peers)
	: interval     (a_interval),
	  min_interval (a_min_interval),
	  complete     (a_complete),
	  incomplete   (a_incomplete),
	  peers        (a_peers)
    {
	/* Left blank. */
    }

    TrackerResponse::
    ~TrackerResponse()
    {
	if (peers) delete peers;
    }
		
    AutoStr
    TrackerResponse::
    description() const
    {
	AutoStr desc;
	asprintf(desc, "[TrackerResponse | Peers: %lu]", (long unsigned int)peers->list().size());
	return desc;
    }

    AutoStr
    TrackerResponse::
    longDescription() const
    {
	AutoStr desc;
	asprintf(desc, "[TrackerResponse | Size: %.f | Interval: %.f | Min Interval: %.f | Seeders: %u | Leechers: %u | List size: %lu]",
		 dataSize().value(),
		 interval.value(),
		 min_interval.value(),
		 complete,
		 incomplete,
		 (long unsigned int)peers->list().size());

	return desc;
    }
}
       
		
