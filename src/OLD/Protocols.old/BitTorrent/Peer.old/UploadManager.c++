#include "Peer/UploadManager.h++"
#include "Peer.h++"
#include "Transfer.h++"

//#include <algorithm>
#include <cassert>
#include <cstdio>

namespace Peer
{
    using ::Transfer::ChunkTransfer;

    signed int UploadManager::DEFAULT_MAX_UPLOADS = 4;
    bool UploadManager::ALLOW_REPEATED_REQUESTS = false;
    bool UploadManager::ALLOW_REQUESTS_FOR_UNOBTAINED_CHUNKS = false;
    bool UploadManager::ALLOW_QUEUING_UNOBTAINED_CHUNKS = false;

    UploadManager::
    UploadManager(Peer& p)
	: _peer(p),
	  _uploads(),
	  _newUploads(),
	  _queuedUploads(),
	  _acceptedUploadRequests(),
	  _maxUploads(DEFAULT_MAX_UPLOADS)
    {
	/* Left blank. */
    }

    bool
    UploadManager::
    requestAlreadyUploading(const ChunkRequest& cr) const
    {
	std::set<ChunkTransferHandle*>::const_iterator it;
	const ChunkTransfer* ct;
	
	for (it = _uploads.begin(); it != _uploads.end(); it++)
	{
	    ct = (*it)->transfer;
	    if ((ct->receiver == cr.peer) && (ct->chunk == cr.chunk))
		return true;
	}

	// unnecessary; anything in _newUploads should be in _uploads too
	/*
	for (it = _newUploads.begin(); it != _newUploads.end(); it++)
	{
	    ct = it->transfer;
	    if ((ct->receiver == cr.peer) && (ct->chunk == cr.chunk))
		return true;
	}
	*/
	return false;
    }

    bool
    UploadManager::
    requestAlreadyQueued(const ChunkRequest& cr) const
    {
	std::list<ChunkRequest>::const_iterator it;
	for (it = _queuedUploads.begin(); it != _queuedUploads.end(); it++)
	    if (cr == *it)
		return true;

	return false;
    }

    bool
    UploadManager::
    requestAlreadyAccepted(const ChunkRequest& cr) const
    {
	return (_acceptedUploadRequests.count(cr) >= 1);
    }


    void
    UploadManager::
    allocateBandwidth(void)
    {
	uint numUploads = _uploads.size();
	Bandwidth allocatedBandwidthPerTransfer = _peer.maxUpstream() / (float) numUploads;

	foreach (ChunkTransferHandle* cth, _uploads)
	{
	    cth->changeBandwidth(allocatedBandwidthPerTransfer);
	}
    }

    void
    UploadManager::
    processNewUploads(void)
    {
	if (_newUploads.empty()) return;

	allocateBandwidth();
	_newUploads.clear();
    }

    void
    UploadManager::
    processQueuedUploads(void)
    {
	if (_queuedUploads.empty()) return;
	if ((signed int)_uploads.size() >= _maxUploads) return;

	ChunkRequest& cr = _queuedUploads.front();
	sendChunk(cr);
	_queuedUploads.pop_front();
    }

    void
    UploadManager::
    sendChunk(ChunkRequest& cr)
    {
	assert(_peer.hasChunk(cr.chunk));
	assert(not requestAlreadyUploading(cr));
	assert(not requestAlreadyAccepted(cr));

	_acceptedUploadRequests.insert(cr);

//	if (requestAlreadyQueued(cr))
//	    _queuedUploads.erase(cr);

	_peer.sendChunk(cr);
    }

    void 
    UploadManager::
    queueChunk(ChunkRequest& cr)
    {
	if (not ALLOW_QUEUING_UNOBTAINED_CHUNKS)
	    assert(_peer.hasChunk(cr.chunk));

	assert(not requestAlreadyUploading(cr));
	assert(not requestAlreadyQueued(cr));
	assert(not requestAlreadyAccepted(cr));

	_queuedUploads.push_back(cr);
	
	_peer.sendChunkRequestQueued(cr);
    }

    void
    UploadManager::
    denyChunk(ChunkRequest& cr)
    {
	/* Left blank. */
    }

    void
    UploadManager::
    cancelQueuedChunk(ChunkRequest& cr)
    {
	assert(not requestAlreadyUploading(cr));
	assert(requestAlreadyQueued(cr));
	assert(not requestAlreadyAccepted(cr));

	_queuedUploads.remove(cr);
	_peer.sendQueuedChunkRequestCanceled(cr);
    }

    void
    UploadManager::
    process(void)
    {
	if (_uploads.empty() || _queuedUploads.empty())
	    return;

	processNewUploads();
	processQueuedUploads();
	assert(availableUpstream() >= 0);
    }

    void
    UploadManager::
    chunkRequested(ChunkRequest& cr)
    {
	if (!_peer.hasChunk(cr.chunk))
	{
	    printf("[PUM]\tPeer %u received a request for a chunk it doesn't have!\n",
		   _peer.id);

	    assert(ALLOW_REQUESTS_FOR_UNOBTAINED_CHUNKS);
	}
	
	if (decideSendChunkNow(cr))
	    sendChunk(cr);
	else if (decideQueueChunk(cr))
	    queueChunk(cr);
	else
	    denyChunk(cr);
    }

    bool
    UploadManager::
    decideSendChunkNow(const ChunkRequest& cr) const
    {
	
	if ((signed int)_uploads.size() >= _maxUploads)
	    return false;
	
	assert(not requestAlreadyUploading(cr));
	
	if (not ALLOW_REPEATED_REQUESTS)
	{
	    assert(not requestAlreadyQueued(cr));
	    assert(not requestAlreadyAccepted(cr));

	    if (_queuedUploads.empty())
		return true;
	    else
		return false;

	}
	else // ALLOW_REPEATED_REQUESTS is true
	{
	    if (requestAlreadyQueued(cr)) return false;
	    if (requestAlreadyAccepted(cr)) return false;
	    if (not _queuedUploads.empty()) return false;
	    
	    return true;
	}
    }

    bool
    UploadManager::
    decideQueueChunk(const ChunkRequest& cr) const
    {
	if (not ALLOW_REPEATED_REQUESTS)
	{
	    assert(not requestAlreadyQueued(cr));
	    assert(not requestAlreadyAccepted(cr));
	}

	return true;
    }

    Bandwidth
    UploadManager::
    availableUpstream(void) const
    {
	Bandwidth max = _peer.maxUpstream();
	Bandwidth used = 0;

	foreach (ChunkTransferHandle* cth, _uploads)
	{
	    used += cth->transfer->bandwidth();
	}

	return max - used;
    }

    uint 
    UploadManager::
    numUploads(void) const
    {
	return _uploads.size();
    }

    void
    UploadManager::
    uploadStarted(ChunkTransferHandle* cth)
    {
	ChunkTransfer* ct = cth->transfer;
	ChunkRequest cr(ct->receiver, ct->chunk);
	
	assert(not requestAlreadyUploading(cr));
	assert(not requestAlreadyQueued(cr));
	assert(requestAlreadyAccepted(cr));

	_acceptedUploadRequests.erase(cr);
	
	_newUploads.insert(cth);
	_uploads.insert(cth);

	process();
    }

    void
    UploadManager::
    uploadBandwidthChanged(ChunkTransferHandle* cth)
    {
	ChunkTransfer* ct = cth->transfer;
	ChunkRequest cr(ct->receiver, ct->chunk);

	assert(_uploads.count(cth) == 1);
	assert(requestAlreadyUploading(cr));
	assert(not requestAlreadyQueued(cr));
	assert(not requestAlreadyAccepted(cr));

	process();
    }

    void
    UploadManager::
    uploadCanceled(ChunkTransferHandle* cth)
    {
	ChunkTransfer* ct = cth->transfer;
	ChunkRequest cr(ct->receiver, ct->chunk);

	assert(_uploads.count(cth) == 1);
	assert(requestAlreadyUploading(cr));
	assert(not requestAlreadyQueued(cr));
	assert(not requestAlreadyAccepted(cr));

	_uploads.erase(cth);

	process();
    }

    void
    UploadManager::
    uploadCompleted(ChunkTransferHandle* cth)
    {
	ChunkTransfer* ct = cth->transfer;
	ChunkRequest cr(ct->receiver, ct->chunk);

	assert(_uploads.count(cth) == 1);
	assert(requestAlreadyUploading(cr));
	assert(not requestAlreadyQueued(cr));
	assert(not requestAlreadyAccepted(cr));
	
	_uploads.erase(cth);
	
	process();
    }

    signed int
    UploadManager::
    maxUploads(void) const
    {
	return _maxUploads;
    }

    void
    UploadManager::
    setMaxUploads(signed int maxUp)
    {
	_maxUploads = maxUp;
    }
}
	
	    
	

	    
	
