#include "Transfer/Message.h++"
#include "Tracker/TrackerRequest.h++"
#include "Tracker.h++"
#include "Peer/Peer.h++"


#include <cstdio>
#include <cstring>

namespace Tracker
{
    TrackerRequest::
    TrackerRequest(Datasize a_uploaded,
		   Datasize a_downloaded,
		   Datasize a_left,
		   Event a_event,
		   bool a_compact,
		   bool a_no_peer_id,
		   uint a_numwant)
	: info_hash(0),
	  peer_id(),
	  port(0),
	  uploaded(a_uploaded),
	  downloaded(a_downloaded),
	  left(a_left),
	  event(a_event),
	  compact(a_compact),
	  no_peer_id(a_no_peer_id),
	  numwant(a_numwant)
    {
	/* Left blank. (So far...) */
    }

    Datasize
    TrackerRequest::
    dataSize(void) const
    {
	Datasize size = 0;

	char *upStr, *downStr, *leftStr, *numwantStr;
	Datasize upLen, downLen, leftLen, numwantLen = 0, eventLen = 0;
	
	asprintf(&upStr,"%u", uploaded);
	asprintf(&downStr, "%u", downloaded);
	asprintf(&leftStr, "%u", left);
	upLen = strlen(upStr);
	downLen = strlen(downStr);
	leftLen = strlen(leftStr);
	free(upStr);
	free(downStr);
	free(leftStr);

	if (numwant >= 0)
	{
	    asprintf(&numwantStr, "%u", numwant);
	    numwantLen = strlen(numwantStr);
	    free(numwantStr);
	}

	switch (event)
	{
	case Start:
	    eventLen = 7;
	    break;
	case Stop:
	    eventLen = 7;
	    break;
	case Complete:
	    eventLen = 8;
	    break;
	default:
	    eventLen = 0;
	}

	size += 20; /* info_hash */
	size += 20; /* peer_id */
	size += 02; /* port */
	size += 01; /* compact */
	size += upLen; /* base-10 ascii of uploaded bytes */
	size += downLen; /* base-10 ascii of downloaded bytes */
	size += leftLen; /* base-10 ascii of number of bytes left */
	size += numwantLen; /* base-10 ascii of number of peers desired (if specified) */
	size += eventLen; /* event string */

	return size;
    }

    std::string
    TrackerRequest::
    description(void) const
    {
	char* desc;
	asprintf(&desc, "[TrackerMessage]");
	std::string descString(desc);
	free(desc);
	return descString;
    }

    std::string
    TrackerRequest::
    longDescription(void) const
    {
	char *desc;
	asprintf(&desc, "[TrackerMessage | Size: %u | Event: %u | Uploaded: %u | Downloaded: %u | Left: %u | Compact: %s | No_Peer_Id: %s | NumWant: %u]", dataSize(), event, uploaded, downloaded, left, compact ? "Yes" : "No", no_peer_id ? "Yes" : "No", numwant);
	std::string descString(desc);
	free(desc);
	return descString;
    }
}
    

