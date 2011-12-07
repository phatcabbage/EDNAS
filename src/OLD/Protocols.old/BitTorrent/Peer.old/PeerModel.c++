#include "Peering.h++"
#include "Transfer.h++"
#include "Simulator.h++"
#include "File.h++"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>

using Transfer::ChunkTransfer;

namespace Peer
{

    PeerModel::
    PeerModel(const Peer *peer)
	: _peers(),
	  _chunksToGet(),
	  _queuedChunks(),
	  _chunksInTransit(),
	  _completedChunks(),
	  _uploads(),
	  _downloads(),
	  _queuedUploads(),
	  _queuedDownloads(),
	  _chunkProviders(),
	  p(const_cast<Peer*>(peer))
    {
	this->initDataStructures();
    }

    void
    PeerModel::
    initDataStructures(void)
    {
	assert(_completedChunks.empty());
	assert(_chunksToGet.empty());

	this->populateChunksToGet();
    }
	
    void
    PeerModel::
    populateChunksToGet(void)
    {
	assert(_chunksToGet.empty());

	File *file = Simulator::instance()->file();
	ChunkID numChunks = file->chunkCount();
    
	for (ChunkID chunkIndex = 0; chunkIndex < numChunks; chunkIndex++)
	    _chunksToGet.insert(_chunksToGet.end(), chunkIndex);
    }    

    void
    PeerModel::
    init(void)
    {
	/* Left blank. */
    }

    void
    PeerModel::
    addPeer(Peer* peer)
    {
	_peers.insert(peer);
    }

    bool
    PeerModel::
    chunkCompleted(ChunkID c) const
    {
	bool result = contains(_completedChunks, c);
	if (result) // if we _do_ have the chunk
	{
	    assert(!contains(_chunksToGet, c));
	    assert(!contains(_queuedChunks, c));
	    assert(!contains(_chunksInTransit, c));
	}

	return result;
    }

    void
    PeerModel::
    addChunkProvider(Peer* peer, ChunkID c)
    {
	std::set<Peer*>* cSet = &(_chunkProviders[c]);

	if (!cSet)
	{
	    printf("Chunkprovider does _NOT_ make lists. Lying bastards.");
	    exit(1);
	}

	cSet->insert(peer);
    };

    std::list<Peer*>*
    PeerModel::
    getChunkProviders(ChunkID c)
    {
	const std::set<Peer*> providers = _chunkProviders[c];
	std::list<Peer*>* result = new std::list<Peer*>(providers.begin(), providers.end());
	return result;
    }

    bool
    PeerModel::
    hasChunkProviders(ChunkID c) 
    {
	printf("[PM]\tChecking if Chunk %u has providers.\n", c);
	uint count = _chunkProviders.count(c);
	const std::set<Peer*>& providers = _chunkProviders[c];
	printf("\t\tIt has %u providers.\n", count);
	bool result = (count > 0);
	printf("\t\tWhich gives us %s.\n", result ? "true" : "false");
	printf("\t\tProvider set is shown to be %s.\n", 
	       (providers.empty()) ? "empty" : "non-empty");
	return result;
    }

    bool
    PeerModel::
    areUploadsQueued(void) const
    {
	return _queuedUploads.empty();
    }

    std::vector<bool>*
    PeerModel::
    getChunkBitfield(void) const
    {
	if (_completedChunks.empty())
	    return new std::vector<bool>;
    
	ChunkID size = Simulator::instance()->file()->chunkCount();
    
	std::vector<bool>* bf = new std::vector<bool>(size);
	foreach(ChunkID c, _completedChunks)
	{
	    (*bf)[c] = true;
	}
	return bf;
    }

    void
    PeerModel::
    parseChunkBitfield(Peer *peer, std::vector<bool>* bf)
    {
	uint size = bf->size();
	for (ChunkID bfIt = 0; bfIt < size; bfIt++)
	{
	    if ((*bf)[bfIt])
		this->addChunkProvider(peer, bfIt);
	}
    }

    void
    PeerModel::
    cheatCompleteChunk(ChunkID c)
    {
	if (_completedChunks.count(c) > 0)
	    return;

	_completedChunks.insert(c);
	_chunksInTransit.erase(c);
	_queuedChunks.erase(c);
	_chunksToGet.erase(c);

	/* We might want to search through _queuedDownloads and _downloads
	   to see if we're downloading it currently. */
    }

    uint
    PeerModel::
    numUploads(void) const
    {
	return _uploads.size();
    }

    uint
    PeerModel::
    numDownloads(void) const
    {
	return _downloads.size();
    }

    bool
    PeerModel::
    isChunkInProgress(ChunkID c) const
    {
	return ((_chunksInTransit.count(c) > 0) || (_queuedChunks.count(c) > 0));
    }

    void
    PeerModel::
    addQueuedDownload(Peer* p, ChunkID c)
    {
	ChunkRequest cr(p, c, Clock::instance()->time());

	assert(!contains(_queuedDownloads, cr));
	assert(_completedChunks.count(c) == 0);
	assert(_chunksInTransit.count(c) == 0);
	assert(_queuedChunks.count(c)    == 0);
	assert(_chunksToGet.count(c)     == 1);
    
	_queuedChunks.insert(c);
	_queuedDownloads.push_back(cr);
    }

    void
    PeerModel::
    addQueuedUpload(Peer* p, ChunkID c)
    {
	assert(contains(_completedChunks, c));
	ChunkRequest cr(p, c, Clock::instance()->time());
	_queuedUploads.push_back(cr);
    }

    void
    PeerModel::
    removeQueuedDownload(Peer* p, ChunkID c)
    {
	ChunkRequest cr(p,c);
	assert(contains(_queuedChunks, c));
	assert(contains(_queuedDownloads, cr));

	_queuedChunks.erase(c);
	_queuedDownloads.remove(cr);
    }

    void
    PeerModel::
    addDownload(ChunkTransferHandle* cth)
    {
	ChunkTransfer* ct = cth->transfer;
	ChunkID chunk = ct->chunk;
	Peer* sender = ct->sender;
	ChunkRequest chunkReq(sender, chunk);

	assert(_completedChunks.count(chunk) < 1);
	assert(_chunksInTransit.count(chunk) < 1);
	assert(_queuedChunks.count(chunk) == 1);
	assert(_chunksToGet.count(chunk) == 1);
	assert(contains(_queuedDownloads, chunkReq));
    
	_queuedChunks.erase(chunk);    
	_chunksInTransit.insert(chunk);
	_queuedDownloads.remove(chunkReq);
	_downloads.push_back(cth);
    }

    void
    PeerModel::
    cancelDownload(ChunkTransferHandle* cth)
    {
	ChunkTransfer* ct = cth->transfer;
	ChunkID chunk = ct->chunk;

	assert(_completedChunks.count(chunk) < 1);
	assert(_chunksInTransit.count(chunk) == 1);
	assert(_queuedChunks.count(chunk) < 1);
	assert(_chunksToGet.count(chunk) == 1);
	assert(contains(_downloads, cth));

	_chunksInTransit.erase(chunk);
	_downloads.remove(cth);
    }

    void
    PeerModel::
    completeDownload(ChunkTransferHandle* cth)
    {
	ChunkTransfer* ct = cth->transfer;
	ChunkID chunk = ct->chunk;

	assert(_completedChunks.count(chunk) < 1);
	assert(_chunksInTransit.count(chunk) == 1);
	assert(_queuedChunks.count(chunk) < 1);
	assert(_chunksToGet.count(chunk) == 1);
	assert(contains(_downloads, cth));
    
	_chunksInTransit.erase(chunk);
	_chunksToGet.erase(chunk);
	_downloads.remove(cth);

	_completedChunks.insert(chunk);
    }

    void
    PeerModel::
    addUpload(ChunkTransferHandle* cth)
    {
	printf("[PM]\tPeer %u adding upload...\n", p->id);
	ChunkTransfer* ct = cth->transfer;
	ChunkRequest chunkReq(ct->receiver, ct->chunk);

	_queuedUploads.remove(chunkReq);
	_uploads.push_back(cth);
    }
 
    void
    PeerModel::
    cancelUpload(ChunkTransferHandle* cth)
    {
	assert(contains(_uploads, cth));
	_uploads.remove(cth);
    }

    void
    PeerModel::
    completeUpload(ChunkTransferHandle* cth)
    {
	assert(contains(_uploads, cth));
	_uploads.remove(cth);
    }

    void
    PeerModel::
    removeQueuedUpload(Peer* p, ChunkID c)
    {
	ChunkRequest cr(p,c);
	std::list<ChunkRequest>::iterator quIt;
	quIt = std::find(_queuedUploads.begin(), _queuedUploads.end(), cr);
	assert(quIt != _queuedUploads.end());

	_queuedUploads.erase(quIt);
    }

    uint
    PeerModel::
    chunksLeft(void) const
    {
	return _completedChunks.size();
    }
    
    const std::set<ChunkID>&
    PeerModel::
    chunksToGet(void) const
    {
	return _chunksToGet;
    }

    ChunkRequest
    PeerModel::
    nextQueuedUpload(void)
    {
	assert(!_queuedUploads.empty());

	ChunkRequest result = _queuedUploads.front();
	_queuedUploads.pop_front();

	return result;
    }

    const std::set<Peer*>&
    PeerModel::
    peers(void) const
    {
	return this->_peers;
    }

    const std::map<ChunkID, std::set<Peer*> >&
    PeerModel::
    chunkProviders(void) const
    {
	return _chunkProviders;
    }

    bool
    PeerModel::
    contains(const std::set<ChunkID>& s, ChunkID c)
    {
	std::set<ChunkID>::const_iterator foundIt;
	foundIt = s.find(c);
	return (foundIt != s.end());
    }

    bool
    PeerModel::
    contains(const std::list<ChunkTransferHandle*>& l,
	     const ChunkTransferHandle* cth)
    {
	std::list<ChunkTransferHandle*>::const_iterator lIt;
	lIt = std::find(l.begin(), l.end(), cth);
	return (lIt != l.end());
    }

    bool
    PeerModel::
    contains(const std::list<ChunkRequest>& l, const ChunkRequest& cr)
    {
	std::list<ChunkRequest>::const_iterator lIt;
	lIt = std::find(l.begin(), l.end(), cr);
	return (lIt != l.end());
    }

    Bandwidth
    PeerModel::
    availableUpstream(void) const
    {
	Bandwidth available = p->peerGroup->maxUp();

	foreach (ChunkTransferHandle* cth, _uploads)
	{
	    Bandwidth b = cth->transfer->bandwidth();
	    available -= b;
	}

	assert(available >= 0);
	return available;
    }

    Bandwidth
    PeerModel::
    availableDownstream(void) const
    {
	Bandwidth available = p->peerGroup->maxDown();
    
	foreach (ChunkTransferHandle* cth, _downloads)
	{
	    Bandwidth b = cth->transfer->bandwidth();
	    available -= b;
	}

	assert(available >= 0);
	return available;
    }

}
