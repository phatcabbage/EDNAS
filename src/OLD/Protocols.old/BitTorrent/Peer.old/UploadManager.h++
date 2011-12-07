#ifndef __UPLOADMANAGER_HXX__
#define __UPLOADMANAGER_HXX__

#include "SEDNAS.h++"

#include <list>
#include <set>

namespace Transfer
{
    struct ChunkTransferHandle;
}

namespace Peer
{
    class ChunkRequest;
    class Peer;

    using ::Transfer::ChunkTransferHandle;

    class UploadManager
    {
    private:
	static signed int DEFAULT_MAX_UPLOADS;
	static bool ALLOW_REPEATED_REQUESTS;
	static bool ALLOW_REQUESTS_FOR_UNOBTAINED_CHUNKS;
	static bool ALLOW_QUEUING_UNOBTAINED_CHUNKS;

	Peer& _peer;

	/**
	 * Contains a the ChunkTransferHandle*'s corresponding to the currently
	 * running uploads.
	 */
	std::set<ChunkTransferHandle*> _uploads;

	/**
	 * Contains ChunkTransferHandle*'s that correspond to newly-started
	 * uploads. This gives the UploadManager an easy-to-find set of 
	 * uploads that might need to be initialized, e.g. setting bandwidth
	 * and such.
	 * Handles found here are also found in the _uploads set.
	 */
	std::set<ChunkTransferHandle*> _newUploads;

	/**
	 * Contains a double-ended queue of the upload requests that we've queued to
	 * send to other peers. 
	 */
	std::list<ChunkRequest> _queuedUploads;

	/**
	 * This contains requests that we've accepted (and begun to transfer)
	 * but haven't received a UploadStartedMessage for yet.
	 */
	std::set<ChunkRequest> _acceptedUploadRequests;

	/**
	 * This contains a set of all the peers that we have currently choked.
	 */
	std::set<Peer*> _chokees;

	bool requestAlreadyUploading(const ChunkRequest& cr) const;
	bool requestAlreadyQueued(const ChunkRequest& cr) const;
	bool requestAlreadyAccepted(const ChunkRequest& cr) const;

    protected:
	signed int _maxUploads;

	void allocateBandwidth(void);
	void processNewUploads(void);
	void processQueuedUploads(void);

	bool decideSendChunkNow(const ChunkRequest& cr) const;
	bool decideQueueChunk(const ChunkRequest& cr) const;
	
	void sendChunk(ChunkRequest& cr);
	void queueChunk(ChunkRequest& cr);
	void denyChunk(ChunkRequest& cr);

	void cancelQueuedChunk(ChunkRequest& cr);

    public:
	UploadManager(Peer&  p);
	
	void process(void);

	void chunkRequested(ChunkRequest& cr);

	Bandwidth availableUpstream(void) const;
	uint numUploads(void) const;

	void uploadStarted(ChunkTransferHandle *cth);
	void uploadBandwidthChanged(ChunkTransferHandle *cth);
	void uploadCanceled(ChunkTransferHandle *cth);
	void uploadCompleted(ChunkTransferHandle *cth);

	signed int maxUploads(void) const;
	void setMaxUploads(signed int maxUp);
    };
}

#endif // __UPLOADMANAGER_HXX__
