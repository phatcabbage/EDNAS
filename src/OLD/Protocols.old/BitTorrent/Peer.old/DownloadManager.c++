#include "Peer/DownloadManager.h++"
#include "Peer.h++"
#include "Transfer.h++"
#include "Simulator.h++"
#include "File.h++"

#include <cassert>
#include <cstdio>
#include <cstdlib> // random()
#include <list>

namespace Peer
{
    using ::Transfer::ChunkTransfer;

    const uint DownloadManager::DEF_MAX_DOWNLOADS = 4;
    const uint DownloadManager::DEF_MAX_PENDING_REQUESTS = 12;
    const Time DownloadManager::DEF_MAX_REQUEST_WAIT = 10;
    
    DownloadManager::
    DownloadManager(Peer& p)
	: _peer(p),
	  _completedChunks(),
	  _downloads(),
	  _requests(),
	  _chunkProviders(), 
	  _requestsPerChunk(),
	  _downloadsPerChunk(),
	  _chokers(),
	  _maxDownloads(DEF_MAX_DOWNLOADS),
	  _maxPendingRequests(DEF_MAX_PENDING_REQUESTS),
	  _maxRequestWait(DEF_MAX_REQUEST_WAIT),
	  _numChunks(Simulator::instance()->file()->chunkCount()),
	  _chunkStatus(new ChunkStatus[_numChunks])
    {
	/* Left blank. */
    }
		     
    DownloadManager::
    ~DownloadManager()
    {
	if (_chunkStatus) delete _chunkStatus;
    }

    void
    DownloadManager::
    allocateBandwidth(void)
    {
	uint numDownloads = _downloads.size();
	Bandwidth max = _peer.maxDownstream();
	Bandwidth allocatedDownstreamPerUpload = max / (float) numDownloads;
	
	foreach(ChunkTransferHandle* cth, _downloads)
	{
	    cth->changeBandwidth(allocatedDownstreamPerUpload);
	}
    }

    void
    DownloadManager::
    attemptMoreDownloads(void)
    {
	uint numDownloads = _downloads.size();
	uint numPendingRequests = _requests.size();
	std::list<ChunkRequest> attempts;
	for (ChunkID index = 0; index < _numChunks; index++)
	{
	    if ((numDownloads < _maxDownloads) && (numPendingRequests < _maxPendingRequests))
	    {
		if (isIncomplete[index])
		{
		    Peer* p = findProvider((ChunkID)index);
		    if (p)
		    {
			attempts.push_back(ChunkRequest(p,index));
			numDownloads++;
			numPendingRequests++;
		    }
		}
	    }
	}

	foreach(ChunkRequest& cr, attempts)
	{
	    requestChunk(cr);
	}
    }		

    Peer*
    DownloadManager::
    findProvider(ChunkID c)
    {
	Peer* provider = NULL;
	const std::set<Peer*>& providers = _chunkProviders[c];
	if (providers.empty())
	    return provider;

	uint numProviders = providers.size();
	uint providerIndex = (uint) random() % numProviders;
	
	std::set<Peer*>::const_iterator it;
	for (it = providers.begin();providerIndex--; it++);
	provider = (*it);
	return provider;
    }

    void
    DownloadManager::
    addProvider(Peer* p, ChunkID c)
    {
	std::set<Peer*>& providers = _chunkProviders[c];
	providers.insert(p);
    }

    bool
    DownloadManager::
    isIncomplete(ChunkID c) const
    {
	bool result = (_chunkStatus[c] == INCOMPLETE);
	return result;
    }

    bool
    DownloadManager::
    isRequested(ChunkID c) const
    {
	bool result = (_chunkStatus[c] & REQUESTED);
	return result;
    }
    
    bool
    DownloadManager::
    isRequested(const ChunkRequest& cr) const
    {
	bool result = (_unansweredRequests.count(cr) > 0);
	if (result)
	    assert(isRequested(cr.chunk));
	return result;
    }

    void
    DownloadManager::
    setRequested(ChunkID c, bool b)
    {
	if (b) 
	    _chunkStatus[c] |= REQUESTED;
	else
	    _chunkStatus[c] &= ~REQUESTED;
    }

    bool
    DownloadManager::
    isDownloading(ChunkID c) const
    {
	bool result = (_chunkStatus[c] & DOWNLOADING);
	return result;
    }

    bool
    DownloadManager::
    isDownloading(const ChunkRequest& cr) const
    {
	ChunkTransfer *ct;
	std::set<ChunkTransferHandle*>::iterator it;
	for (it = _downloads.begin(); it != _downloads.end(); it++)
	{
	    ct = (*it)->transfer;
	    if ((cr.peer == ct->sender) && (cr.chunk == ct->chunk))
	    {
		assert(isDownloading(cr.chunk));
		return true;
	    }
	}
	assert(not isDownloading(cr.chunk));
	return false;
    }

    bool
    DownloadManager::
    isDownloading(ChunkTransferHandle* cth) const
    {
	bool result = (_downloads.count(cth) > 0);
	if (result)
	    assert(isDownloading(ChunkRequest(cth->transfer->sender, cth->transfer->chunk)));
	return result;
    }

    bool
    DownloadManager::
    setDownloading(ChunkID c, bool b)
    {
	if (b)
	    _chunkStatus[c] |= DOWNLOADING;
	else
	    _chunkStatus[c] &= ~DOWNLOADING;
    }

    bool
    DownloadManager::
    isCompleted(ChunkID c) const
    {
	return (_chunkStatus[c] & COMPLETED);
    }

    void
    DownloadManager::
    setCompleted(ChunkID c, bool b)
    {
	if (b)
	    _chunkStatus[c] |= COMPLETED;
	else
	    _chunkStatus[c] &= ~COMPLETED;
    }

    void
    DownloadManager::
    process(void)
    {
	if ((signed int)_downloads.size() < _maxDownloads)
	    attemptMoreDownloads();

	if (_downloads.size() > 0)
	    allocateBandwidth();

	assert(availableDownstream() >= 0);
    }

    void
    DownloadManager::
    peerBitfield(Peer* p, const std::vector<bool>* bf)
    {
	uint size = bf->size();
	for (ChunkID index = 0; index < size; index++)
	    if ((*bf)[index])
		addProvider(p, index);
    }

    void
    DownloadManager::
    peerHasChunk(Peer* p, ChunkID c)
    {
	addProvider(p,c);
    }		

/*
    void
    DownloadManager::
    chunkRequestQueued(ChunkRequest& cr)
    {
	assert(isRequested(cr.chunk));
	assert(_unansweredRequests.count(cr) > 0);
	uint requests = _requestedChunkCount[cr.chunk];
	assert(requests > 0);

	// decrement the number of requests we have for that chunk
	// and if we have no more, save memory by releasing it from the map
	_requestedChunkCount[cr.chunk] = --requests;
	if (requests == 0)
	{
	    _requestedChunkCount.erase(cr.chunk);
	    setRequested(cr.chunk, false);
	}

	// note that the request has been answered
	_unansweredRequests.erase(cr);
	
	// and if we've completed the chunk, cancel the request
	if (isCompleted(cr.chunk))
	{
	    cancelQueuedChunkRequest(cr);
	    return;
	}
	
	_queuedDownloads.insert(cr);
	setQueued(cr.chunk, true);
	_queuedChunkCount[cr.chunk]++;
    }

    void
    DownloadManager::
    chunkRequestCanceled(ChunkRequest& cr)
    {
	assert(isRequested(cr.chunk));
	assert(_unansweredRequests.count(cr) > 0);
	uint requests = _requestedChunkCount[cr.chunk];
	assert(requests > 0);

	_requestedChunkCount[cr.chunk] = --requests;
	if (requests == 0)
	{
	    _requestedChunkCount.erase(cr.chunk);
	    setRequested(cr.chunk, false);
	}

	_unansweredRequests.erase(cr);
    }
	
    void
    DownloadManager::
    queuedChunkRequestCanceled(ChunkRequest& cr)
    {
	assert(isQueued(cr));
	uint queuedRequests = _queuedChunkCount[cr.chunk];
	assert(requests > 0);

	_queuedChunkCount[cr.chunk] = --queuedRequests;
	if (queuedRequests == 0)
	{
	    _queuedChunkCount.erase(cr);
	    setQueued(cr.chunk, false);
	}

	_queuedDownloads.erase(cr);
    }
*/

    void
    DownloadManager::
    downloadStarted(ChunkTransferHandle* cth)
    {
	ChunkRequest cr(cth->transfer->sender, cth->transfer->chunk);
	ChunkID c = cth->transfer->chunk;

	if (isRequested(cr))
	{
	    assert(_requestedChunkCount[c] > 0);
	    assert(_unansweredRequests.count(cr) > 0);

	    _requestedChunkCount[c]--;
	    _unansweredRequests.erase(cr);
	    if (_requestedChunkCount[c] == 0)
		setRequested(c, false);
	}
	

	if (isCompleted(c))
	{
	    cancelDownload(cth);
	    return;
	}

	_downloadingChunkCount[c]++;

	_downloads.insert(cth);
	setDownloading(c, true);

	process();
    }

    void
    DownloadManager::
    downloadBandwidthChanged(ChunkTransferHandle* cth)
    {
	assert(isDownloading(cth));
	
	process();
    }

    void
    DownloadManager::
    downloadCanceled(ChunkTransferHandle* cth)
    {
	Chunk c = cth->transfer->chunk;
	assert(isDownloading(cth));
	assert(_downloadingChunkCount[c] > 0);

	_downloadingChunkCount[c]--;

	if (_downloadingChunkCount[c] == 0)
	{
	    setDownloading(c, false);
	    _downloadingChunkCount.erase(c);
	}
	_downloads.erase(cth);
	process();
    }
	
    void
    DownloadManager::
    downloadCompleted(ChunkTransferHandle* cth)
    {
	assert(isDownloading(cth));
	assert(_downloadingChunkCount[c] > 0);
	
	_downloadingChunkCount[c]--;
	if (_downloadingChunkCount[c] == 0)
	{
	    setDownloading(c, false);
	    setCompleted(c, true);
	}
	else
	{
	    foreach(ChunkTransferHandle* cth_temp, _downloads)
	    {
		if (cth_temp->transfer->chunk == c)
		    cancelDownload(cth_temp);
	    }
	}

	if (isQueued(c))
	{
	    foreach(ChunkRequest& cr_temp, _queuedDownloads)
	    {
		if (cr_temp.chunk == c)
		    cancelQueuedChunkRequest(cr);
	    }
	}

	_completedChunks.insert(c);
	setCompleted(c);
	_downloads.erase(cth);
	process();
    }

    bool
    DownloadManager::
    hasChunk(ChunkID c) const
    {
	return (_chunkStatus[c] == COMPLETED);
    }

    uint
    DownloadManager::
    numDownloads(void) const
    {
	return _downloads.size();
    }

    Bandwidth
    DownloadManager::
    availableDownstream(void) const
    {
	Bandwidth max = _peer.maxDownstream();
	Bandwidth used = 0;

	foreach (ChunkTransferHandle* cth, _downloads)
	{
	    used += cth->transfer->bandwidth();
	}

	return max - used;
    }

    void
    DownloadManager::
    bestowChunk(ChunkID c)
    {
	if (isDownloading(c))
	{
	    std::list<ChunkTransferHandle*> chunkDownloads;
	    findTransferHandleForChunk(c, chunkDownloads);
	    foreach (ChunkTransferHandle* cth, chunkDownloads)
	    {
		cancelDownload(cth);
	    }
	}
	
	if (isQueued(c))
	{
	    std::list<ChunkRequest*> chunkRequests;
	    findQueuedRequestsForChunk(c, chunkRequests);
	    foreach (ChunkRequest& cr, chunkRequest)
	    {
		cancelRequest(cr);
	    }
	}

	_chunkStatus[c] = COMPLETED;
    }
}
