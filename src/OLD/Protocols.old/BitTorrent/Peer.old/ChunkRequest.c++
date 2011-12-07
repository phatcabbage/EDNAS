#include "Peer.h++"

namespace Peer
{
    ChunkRequest::
    ChunkRequest(Peer* p, ChunkID c, Time cr)
	: peer(p),
	  chunk(c),
	  creation(cr)
    {
	/* Left blank. */
    }

    ChunkRequest::
    ChunkRequest(const ChunkRequest& cr)
	: peer(cr.peer), 
	  chunk(cr.chunk),
	  creation(cr.creation)
    {
	/* Left blank. */
    }

    bool
    ChunkRequest::
    operator==(const ChunkRequest& o) const
    {
	return (peer == o.peer && chunk == o.chunk);
    }

    bool 
    ChunkRequest::
    operator==(const ChunkRequest* o) const
    {
	return (peer == o->peer && chunk == o->chunk);
    }

    bool
    ChunkRequest::
    operator<(const ChunkRequest& o) const
    {
	if (peer < o.peer) return true;

	if (peer == o.peer)
	    return (chunk < o.chunk);

	return (peer < o.peer);
    }

    bool
    ChunkRequest::SortByPeer::operator()(const ChunkRequest& a, const ChunkRequest& b) const
    {
	return (a.peer < b.peer);
    }

    bool
    ChunkRequest::SortByPeer::operator()(const ChunkRequest* a, const ChunkRequest* b) const
    {
	return (a->peer < b->peer);
    }

    bool
    ChunkRequest::SortByChunkID::operator()(const ChunkRequest& a, const ChunkRequest& b) const
    {
	return (a.chunk < b.chunk);
    }

    bool
    ChunkRequest::SortByChunkID::operator()(const ChunkRequest* a, const ChunkRequest* b) const
    {
	return (a->chunk < b->chunk);
    }

    bool
    ChunkRequest::SortByCreationTime::operator()(const ChunkRequest& a, const ChunkRequest& b) const
    {
	return (a.creation < b.creation);
    }

    bool
    ChunkRequest::SortByCreationTime::operator()(const ChunkRequest* a, const ChunkRequest* b) const
    {
	return (a->creation < b->creation);
    }
    
}

