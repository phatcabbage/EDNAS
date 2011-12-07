#ifndef __TRACKERRESPONSE_HXX__
#define __TRACKERRESPONSE_HXX__

#include "SEDNAS.h++"
#include "Transfer/Message.h++"

namespace Tracker
{
    class ContactList;

    struct TrackerResponse
    {
	const 
	Time 
	interval;
	
	const
	Time 
	min_interval;

	const 
	uint 
	complete;

	const 
	uint 
	incomplete;

	ContactList* const
	peers;
	
	Data 
	dataSize() const;

	TrackerResponse(const Time& a_interval,
			const Time& a_min_interval,
			      uint  a_complete,
			      uint  a_incomplete,
			ContactList* a_peers);

	~TrackerResponse();

	AutoStr 
	description() const;
	AutoStr
	longDescription() const;
    };
}

#endif // __TRACKERRESPONSE_HXX__
