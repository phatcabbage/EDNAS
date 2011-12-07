#include "../Peer.h++"
#include "../Transfer.h++"
#include "../Tracker.h++"

#include "./DownloadManager.h++"
#include "./PeeringManager.h++"
#include "./UploadManager.h++"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <set>
#include <vector>

namespace Peer
{
    PeerController::
    PeerController(Peer& peer)
	: _peer(peer),
	  _downloadMgr(new DownloadManager(peer)),
	  _peeringMgr(new PeeringManager(peer)),
	  _uploadMgr(new UploadManager(peer))
    {
	/* Left blank. */
    }

    PeerController::
    ~PeerController()
    {
	if (_downloadMgr) delete _downloadMgr;
	if (_peeringMgr) delete _peeringMgr;
	if (_uploadMgr) delete _uploadMgr;
    }

    void
    PeerController::
    start(void)
    {
	_peeringMgr->start();
    }

    void
    PeerController::
    handshake(Peer* s)
    {
	_peeringMgr->handshake(s);
    }

    void
    PeerController::
    handshakeResponse(Peer* s)
    {
	_peeringMgr->handshakeResponse(s);
    }

    void
    PeerController::
    trackerResponse(std::vector<Peer*>* cList)
    {
	_peeringMgr->trackerResponse(cList);
/*
	std::vector<Peer*> newContacts;
	std::back_insert_iterator<std::vector<Peer*> > newContactIt(newContacts);
    
	const std::set<Peer*>& peerSet = p->model->peers();

	Peer::Sorter psort;
	std::sort(contacts->begin(), contacts->end(), psort);

	std::set_difference(contacts->begin(), contacts->end(),
			    peerSet.begin(), peerSet.end(),
			    newContactIt);

	_waitingOnTrackerRequest = false;


	foreach(Peer* c, newContacts)
	{
	    p->sendHandshake(c);
	    _unansweredHandshakes.insert(std::make_pair<Peer*,Time>
					 (c,Clock::instance()->time()));
	}

	if (_unansweredHandshakes.size() + peerSet.size() < DESIRED_NUM_PEERS)
	{
	    PeerTimerEntry *pte = new PeerTimerEntry(p, &Peer::sendTrackerRequest);
	    Timer::instance()->create(Clock::instance()->time() + 
				      MIN_TIME_BETWEEN_TRACKER_REQUESTS, pte);
	}

	this->updateDownloads();
*/
    }

    void
    PeerController::
    bitfield(Peer* s, const std::vector<bool>* bf)
    {
	_downloadMgr->peerBitfield(s, bf);
    }

    void
    PeerController::have(Peer* s, ChunkID c)
    {
	_downloadMgr->peerHasChunk(s, c);
    }

    void
    PeerController::
    chunkRequest(ChunkRequest& cr)
    {
	_uploadMgr->chunkRequested(cr);
    }
/*
    void
    PeerController::
    chunkRequestAnswered(Peer* s, ChunkID c)
    {
	ChunkRequest cr(s,c);
	std::set<ChunkID>& chunksFromPeer = _unansweredChunkRequestsForPeers[s];
	std::set<ChunkID>::iterator cpIt;
	std::set<ChunkRequest, ChunkRequest::SortByCreationTime>::iterator crIt;

	cpIt = chunksFromPeer.find(c);
	crIt = _unansweredChunkRequests.find(cr);

	assert(cpIt != chunksFromPeer.end());
	assert(crIt != _unansweredChunkRequests.end());

	chunksFromPeer.erase(cpIt);
	_unansweredChunkRequests.erase(crIt);
    }
*/  

    void
    PeerController::
    chunkRequestQueued(ChunkRequest& cr)
    {
	_downloadMgr->chunkRequestQueued(cr);
    }

    void
    PeerController::
    chunkRequestCanceled(ChunkRequest& cr)
    {
	_downloadMgr->chunkRequestCanceled(cr);
    }

    void
    PeerController::
    queuedChunkRequestCanceled(ChunkRequest& cr)
    {
	_downloadMgr->queuedChunkRequestCanceled(cr);
    }

/*
    void
    PeerController::
    checkUnansweredHandshakes(void)
    {
	printf("[PC]\tPeer %u is checking for unanswered handshakes.\n", p->id);
	Time now = Clock::instance()->time();
	std::map<Peer*, Time>::iterator handIt;
    
	for (handIt = _unansweredHandshakes.begin(); 
	     handIt != _unansweredHandshakes.end(); 
	     handIt++)
	{
	    Time timeElapsed = now - (*handIt).second;
	    if (timeElapsed > MAX_HANDSHAKE_WAIT)
	    {
		printf("\t\tUnanswered handshake found.");
		_unansweredHandshakes.erase(handIt);
	    }
	}
    }

    void
    PeerController::
    checkUnansweredChunkRequests(void)
    {
	printf("[PC]\tPeer %u is checking for unanswered chunk requests.\n", p->id);
	Time now = Clock::instance()->time();
	std::set<ChunkRequest>::iterator crIt = _unansweredChunkRequests.begin();

	while (crIt != _unansweredChunkRequests.end())
	{
	    const ChunkRequest& cr = *crIt;
	    Time elapsedTime = now - cr.creation;

	    if (elapsedTime > MAX_CHUNKREQUEST_RESPONSE_WAIT)
	    {
		printf("\t\tUnanswered chunk request found.\n");
		std::set<ChunkID>& cSet = _unansweredChunkRequestsForPeers[cr.peer];
		cSet.erase(cr.chunk);
		_unansweredChunkRequests.erase(crIt);
	    }

	    crIt++;
	}
    }

    bool
    PeerController::
    alreadyRequested(ChunkID c) const
    {
	for (std::set<ChunkRequest>::const_iterator it = _unansweredChunkRequests.begin();
	     it != _unansweredChunkRequests.end();
	     it++)
	{
	    if ((*it).chunk == c)
		return true;
	}

	return false;
    }

    void
    PeerController::
    updateDownloads(void)
    {
	printf("Into updateDownloads.\n");

	printf("\tAbout to check unanswered chunk requests.\n");
	checkUnansweredChunkRequests();
	printf("\t\tDone.\n");

	const std::map<ChunkID, std::set<Peer*> >& providerMap = 
	    p->model->chunkProviders();

	std::map<ChunkID, std::set<Peer*> >::const_iterator pIt = 
	    providerMap.begin();

	printf("\tAbout to start looping to find more chunks to download.\n");
	while ((p->model->numDownloads() + _unansweredChunkRequests.size()) <
	       MAX_CONCURRENT_DOWNLOADS)
	{
	    if (pIt == providerMap.end())
		break;

	    ChunkID chunk = pIt->first;
	    const std::set<Peer*>& providers = pIt->second;
	    assert(!providers.empty());
	    pIt++;

	    if (p->model->chunkCompleted(chunk))
		continue;

	    if (p->model->isChunkInProgress(chunk))
		continue;

	    Peer* provider = randomPeer(providers);
	    this->requestChunk(provider, chunk);

	    printf("\t\tRequested chunk %u.\n", chunk);
	}
	printf("\t\tDone.\n");
	printf("\tExiting updateDownloadQueue.\n");
    }

    void
    PeerController::
    updateUploads(void)
    {
	if (p->model->numUploads() >= MAX_CONCURRENT_UPLOADS)
	    return;

	if (!p->model->areUploadsQueued())
	    return;

	ChunkRequest c = p->model->nextQueuedUpload();
	p->sendChunk(c.peer, c.chunk);
    }

    void
    PeerController::
    requestChunk(Peer* provider, ChunkID c)
    {
	assert(!p->model->chunkCompleted(c));
	assert(p->model->hasChunkProviders(c));

	p->requestChunk(provider, c);

	_unansweredChunkRequestsForPeers[provider].insert(c);
	_unansweredChunkRequests.insert(ChunkRequest(provider, c, 
						     Clock::instance()->time()));
    }
*/

    void
    PeerController::
    downloadStarted(ChunkTransferHandle* cth)
    {
	_downloadMgr->downloadStarted(cth);
    }

    void
    PeerController::
    downloadCanceled(ChunkTransferHandle* cth)
    {
	_downloadMgr->downloadCanceled(cth);
    }

    void
    PeerController::
    downloadBandwidthChanged(ChunkTransferHandle* cth, Bandwidth b)
    {
	_downloadMgr->downloadBandwidthChanged(cth);
    }

    void
    PeerController::
    downloadCompleted(ChunkTransferHandle* cth)
    {
	_downloadMgr->downloadCompleted(cth);
    }

    void
    PeerController::
    uploadStarted(ChunkTransferHandle* cth)
    {
	_uploadMgr->uploadStarted(cth);
    }

    void
    PeerController::
    uploadBandwidthChanged(ChunkTransferHandle* cth, Bandwidth b)
    {
	_uploadMgr->uploadBandwidthChanged(cth);
    }

    void
    PeerController::
    uploadCanceled(ChunkTransferHandle* cth)
    {
	_uploadMgr->uploadCanceled(cth);
    }

    void
    PeerController::
    uploadCompleted(ChunkTransferHandle* cth)
    {
	_uploadMgr->uploadCompleted(cth);
    }

    void
    PeerController::
    cheatCompleteChunk(ChunkID c)
    {
	_downloadMgr->bestowChunk(c);
    }

/*
    void
    PeerController::
    reduceUpstream(void)
    {
	printf("[X PC]\t*** reduceUpstream STUB!! We don't do that yet!\n");
    }

    void
    PeerController::
    reduceDownstream(void)
    {
	printf("[X PC]\t*** reduceDownstream STUB!! We don't do that yet!\n");
    }


    Peer*
    PeerController::
    randomPeer(const std::set<Peer*>& peerSet)
    {
	uint size = peerSet.size();
	uint randNo = random();
	uint pos = randNo % size;

	std::set<Peer*>::const_iterator it = peerSet.begin();
	for(uint index = 0; index < pos; index++)
	    it++;

	printf("\t[%u]\n", (*it)->id);
	return (*it);
    }
*/
}
