#ifndef PROTOCOLS__BITTORRENT__TRACKERMESSAGES_HXX__
#define PROTOCOLS__BITTORRENT__TRACKERMESSAGES_HXX__

#include <vector>

#include "Core.h++"
#include "Foundation/Network/NetFwd.h++"

namespace Protocol
{
    namespace BitTorrent
    {
	struct Peer;
	struct Tracker;

	struct TrackerRequest;
	struct TrackerResponse;

	struct TrackerRequest
	{
	    enum {SIZE = 128};
	    TrackerRequest() {}
	    
	    static DataType
	    Size(){ return DataType(SIZE);}
	};

	struct TrackerResponse
	{
	    std::vector<Peer*> Peers;
	    const DataType MsgSize;
	    
	    TrackerResponse(std::vector<Peer*>& v);
	    TrackerResponse(const std::vector<Peer*>& v);
	    
	    static DataType
	    Size();
	};

	typedef Foundation::Net::Message<TrackerRequest>  TrackerRequestMessage;
	typedef Foundation::Net::Message<TrackerResponse> TrackerResponseMessage;
    }
}
	    

#endif // PROTOCOLS__BITTORRENT__TRACKERMESSAGES_HXX__
