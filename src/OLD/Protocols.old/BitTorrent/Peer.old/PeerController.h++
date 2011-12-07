#ifndef __PEERCONTROLLER_HXX__
#define __PEERCONTROLLER_HXX__

#include "SEDNAS.h++"

#include <deque>
#include <vector>

namespace Transfer
{
    struct ChunkTransferHandle;
}

namespace Peer
{
    using ::Transfer::ChunkTransferHandle;
    
    class ChunkRequest;
    class Peer;

    class DownloadManager;
    class PeeringManager;
    class UploadManager;

    class PeerController
    {
    private:
	Peer& _peer;
	
	DownloadManager* _downloadMgr;
	PeeringManager*  _peeringMgr;
	UploadManager*   _uploadMgr;
	
    public:
	PeerController(Peer& p);
	~PeerController();
	
	void start(void);
	
	void trackerResponse(std::vector<Peer*>* cList);
	void handshake(Peer* p);
	void handshakeResponse(Peer* p);
	void bitfield(Peer* b, const std::vector<bool>* bf);
	void choked(Peer* p);
	void unchoked(Peer* p);
	void keepAlive(Peer* p);
	void have(Peer* p, ChunkID c);
	void chunkRequest(ChunkRequest& cr);
	void chunkRequestQueued(ChunkRequest& cr);
	void chunkRequestCanceled(ChunkRequest& cr);
	void queuedChunkRequestCanceled(ChunkRequest& cr);
	void downloadStarted(ChunkTransferHandle* cth);
	void downloadBandwidthChanged(ChunkTransferHandle* cth, Bandwidth b);
	void downloadCanceled(ChunkTransferHandle* cth);
	void downloadCompleted(ChunkTransferHandle* cth);
	void uploadStarted(ChunkTransferHandle* cth);
	void uploadBandwidthChanged(ChunkTransferHandle* cth, Bandwidth b);
	void uploadCanceled(ChunkTransferHandle* cth);
	void uploadCompleted(ChunkTransferHandle* cth);

	void cheatCompleteChunk(ChunkID c);
    };
}

#endif // __PEERCONTROLLER_HXX_
