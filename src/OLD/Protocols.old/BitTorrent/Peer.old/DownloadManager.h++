#ifndef __DOWNLOADMANAGER_HXX__
#define __DOWNLOADMANAGER_HXX__

#include "SEDNAS.h++"
#include "Peer/ChunkRequest.h++"
#include "Timing.h++"

#include <map>
#include <set>
#include <vector>

namespace Transfer
{
    struct ChunkTransferHandle;
}

namespace Peer
{
    class DownloadManager;
    class Peer;

    typedef Peer* PeerHandle;

    using ::Transfer::ChunkTransferHandle;

    typedef void (DownloadManager::*DownloadManagerTimerCallback)(ChunkRequest&);

    class DownloadManager
    {
    private:
	static const uint DEF_MAX_DOWNLOADS;
	static const uint DEF_MAX_PENDING_REQUESTS;
	static const Time DEF_MAX_REQUEST_WAIT;

	enum ChunkStatus
	{
	    INCOMPLETE =	0b0000,
	    REQUESTED = 	0b0001,
	    DOWNLOADING = 	0b0010,
	    COMPLETED = 	0b0100
	};

	Peer& _peer;

	const uint _numChunks;
	ChunkStatus* _chunkStatus;

	std::set<ChunkID> _completedChunks;

	std::set<ChunkTransferHandle*> _downloads;
	std::set<ChunkRequest> _requests;
	std::map<ChunkID, std::set<PeerHandle> > _chunkProviders;

	std::map<ChunkID, uint> _requestsPerChunk;
	std::map<ChunkID, uint> _downloadsPerChunk;

	// Peers that have choked us
	std::set<PeerHandle> _chokers;


    protected:
	uint _maxDownloads;
	uint _maxPendingRequests;
	Time _maxRequestWait;
	
	void allocateBandwidth(void);

	void attemptMoreDownloads(void);
	PeerHandle findProvider(ChunkID c);
	void addProvider(PeerHandle p, ChunkID c);
	
	void requestChunk(ChunkRequest& cr);

	void isIncomplete(ChunkID c) const;

	bool isRequested(ChunkID c) const;
	bool isRequested(const ChunkRequest& cr) const;
	void setRequested(ChunkID c, bool b);

	bool isDownloading(ChunkID c) const;
	bool isDownloading(const ChunkRequest& cr) const;
	bool isDownloading(/* const */ ChunkTransferHandle* cth) const;
	void setDownloading(ChunkID c, bool b);

	bool isCompleted(ChunkID c) const;
	void setCompleted(ChunkID c, bool b);

	void cancelDownload(ChunkTransferHandle* cth);

	void findTransferHandleForChunk(ChunkID c, std::list<ChunkTransferHandle*>& resultList) const;

    public:
	DownloadManager(Peer& p);
	~DownloadManager();

	void process(void);

	void peerBitfield(PeerHandle p, const std::vector<bool>* bf);
	void peerHasChunk(PeerHandle p, ChunkID c);

	void chunkRequestQueued(ChunkRequest& cr);
	void chunkRequestCanceled(ChunkRequest& cr);
	void queuedChunkRequestCanceled(ChunkRequest& cr);

	void downloadStarted(ChunkTransferHandle* cth);
	void downloadBandwidthChanged(ChunkTransferHandle *cth);
	void downloadCanceled(ChunkTransferHandle* cth);
	void downloadCompleted(ChunkTransferHandle* cth);

	bool hasChunk(ChunkID c) const;
	std::vector<bool>* bitfield(void) const;
	uint numDownloads(void) const;
	Bandwidth availableDownstream(void) const;

	void bestowChunk(ChunkID c);
    };
    
    struct DownloadManagerRequestTimerEntry : public Timing::TimerEntry
    {
	DownloadManager *dm;
	DownloadManagerTimerCallback f;
	ChunkRequest cr;
	DownloadManagerRequestTimerEntry(DownloadManager *d, DownloadManagerTimerCallback fun, const ChunkRequest& crq);

	virtual void execute(TimerID);
    };
}


#endif // __DOWNLOADMANAGER_HXX__
