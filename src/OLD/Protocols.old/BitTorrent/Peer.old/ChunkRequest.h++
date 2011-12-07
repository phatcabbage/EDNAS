#ifndef __CHUNKREQUEST_HXX__
#define __CHUNKREQUEST_HXX__

#include "SEDNAS.h++"
#include <set>


namespace Peer
{
    class Peer;

    struct ChunkRequest
    {
    public:
	Peer* const peer;
	const ChunkID chunk;
	const Time creation;
	ChunkRequest(Peer* p, ChunkID c, Time cr = 0);
	ChunkRequest(const ChunkRequest& cr);
	bool operator==(const ChunkRequest& o) const;
	bool operator==(const ChunkRequest* o) const;
	bool operator< (const ChunkRequest& o) const;

	struct SortByPeer
	{
	    bool operator()(const ChunkRequest& a, const ChunkRequest& b) const;
	    bool operator()(const ChunkRequest* a, const ChunkRequest* b) const;
	};
    
	struct SortByChunkID
	{
	    bool operator()(const ChunkRequest& a, const ChunkRequest& b) const;
	    bool operator()(const ChunkRequest* a, const ChunkRequest* b) const;
	};

	struct SortByCreationTime
	{
	    bool operator()(const ChunkRequest& a, const ChunkRequest& b) const;
	    bool operator()(const ChunkRequest* a, const ChunkRequest* b) const;
	};
    };
}
#endif // __CHUNKREQUEST_HXX_
