#ifndef __PEER__PEERMODEL_HXX__
#define __PEER__PEERMODEL_HXX__

#include "../SEDNAS.h++"

#include <list>
#include <map>
#include <set>
#include <vector>

namespace Transfer
{
    struct ChunkTransferHandle;
}

namespace Peer
{
    class ChunkRequest;
    class Peer;
    class PeerController;

    class PeerModel
    {
	friend class Peer;
	friend class PeerController;

    private:
	std::set<Peer*> _peers;
	std::set<ChunkID> _chunksToGet;
	std::set<ChunkID> _queuedChunks;
	std::set<ChunkID> _chunksInTransit;
	std::set<ChunkID> _completedChunks;

	std::list<ChunkTransferHandle*> _uploads;
	std::list<ChunkTransferHandle*> _downloads;
	std::list<ChunkRequest> _queuedUploads;
	std::list<ChunkRequest> _queuedDownloads;
	std::map<ChunkID, std::set<Peer*> > _chunkProviders;

	static bool contains(const std::set<ChunkID>& s, ChunkID c);
	static bool contains(const std::list<ChunkTransferHandle*>& l, 
			     const ChunkTransferHandle* cth);
	static bool contains(const std::list<ChunkRequest>& l, const ChunkRequest& cr);
    

	void initDataStructures(void);
	void populateChunksToGet(void);

    public:
	const Peer* p;

	PeerModel(const Peer* peer);

	void init(void);
	void addPeer(Peer* peer);
	const std::set<Peer*>& peers(void) const;
	bool chunkCompleted(ChunkID c) const;
	void addChunkProvider(Peer* peer, ChunkID c);
	std::list<Peer*>* getChunkProviders(ChunkID c);
	bool hasChunkProviders(ChunkID c);
	std::vector<bool>* getChunkBitfield(void) const;
	void parseChunkBitfield(Peer* peer, std::vector<bool>* bf);
	void completeChunk(ChunkID c);
	void cheatCompleteChunk(ChunkID c);
	uint numUploads(void) const;
	uint numDownloads(void) const;
	void addDownload(ChunkTransferHandle* cth);
	void addUpload(ChunkTransferHandle* cth);
	void addQueuedDownload(Peer* p, ChunkID c);
	void addQueuedUpload(Peer* p, ChunkID c);
	void removeQueuedDownload(Peer* p, ChunkID c);
	void removeQueuedUpload(Peer* p, ChunkID c);
	uint chunksLeft(void) const;
	bool isChunkInProgress(ChunkID c) const;
	const std::set<ChunkID>& chunksToGet(void) const;

	bool areUploadsQueued(void) const;
	ChunkRequest nextQueuedUpload(void);
	const std::map<ChunkID, std::set<Peer*> >& chunkProviders(void) const;
    
	void cancelDownload(ChunkTransferHandle* cth);
	void completeDownload(ChunkTransferHandle* cth);
	void cancelUpload(ChunkTransferHandle* cth);
	void completeUpload(ChunkTransferHandle* cth);
	Bandwidth availableUpstream(void) const;
	Bandwidth availableDownstream(void) const;
    };
}

#endif // __PEERMODEL_HXX__
