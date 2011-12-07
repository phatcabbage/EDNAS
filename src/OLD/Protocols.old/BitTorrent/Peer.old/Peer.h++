//#ifdef __ABSTRACT_PEER__
#ifndef __PEER__PEER_HXX__
#define __PEER__PEER_HXX__

#include "Signaling.h++"
#include "Transfer/Message.h++"
#include "Tracker/TrackerResponse.h++"
#include "Transfer/Transferer.h++"


class Simulator;

namespace Tracker { class Tracker; }
namespace Transfer{ template<typename Sender, typename Receiver, typename Payload> class Message;}

namespace Peer 
{ 
    class Peer
	:  public Transfer::Transferer
    {
    public:
	Peer();
	void receive(Transfer::Message<Tracker::Tracker,Peer, Tracker::TrackerResponse>* m);
	void sendTrackerRequest();
    };
}

#endif // __PEER__PEER_HXX__
//#endif // __ABSTRACT_PEER__

#ifndef __PEER__PEER_HXX__
#define __PEER__PEER_HXX__

#include "SEDNAS.h++"

#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

const uint DEFAULT_MINIMUM_SUFFICIENT_PEERS = 10;
const Time DEFAULT_INSUFFICIENT_PEER_EVENT_DELAY = 20;

namespace Transfer
{
    struct AbstractMessage;
    template<typename S, typename R, typename P> class Message;
}

namespace Tracker
{
    class Tracker;
    class TrackerRequest;
    class TrackerResponse;
}

namespace Peer
{
    class ChunkRequest;
    class PeerGroup;
    class PeerModel;
    class PeerController;

    class Peer
    {
    protected:
	friend class PeerController;

	static 
	PeerID 
	g_uidc;

    public:
	const 
	PeerGroup* 
	peerGroup;
	
	const 
	PeerID 
	id;

	PeerController* 
	controller;
    
	Peer(const PeerGroup* a_pg);
	~Peer();

	void 
	start();
	
	void 
	bestowChunk(ChunkID chunk);

	void 
	receive(Messages::HandshakeMessage* m);
	
	void 
	receive(Messages::HandshakeResponseMessage* m);

	void 
	receive(Messages::BitfieldMessage* m);

	void 
	receive(Messages::ChokeMessage* m);
	
	void 
	receive(Messages::UnchokeMessage* m);
	
	void 
	receive(Messages::KeepAliveMessage* m);

	void 
	receive(Messages::HaveMessage* m);
	
	void 
	receive(Messages::ChunkRequestMessage* m);
	
	void 
	receive(Messages::ChunkRequestQueuedMessage* m);
	
	void
	receive(Messages::ChunkRequestCanceledMessage* m);
	
	void
	receive(Messages::QueuedChunkRequestCanceledMessage* m);

	void
	receive(Tracker::Messages::ResponseMessage* m);

	void
	receive(Transfer::Messages::UploadStartedMessage* m);
	
	void
	receive(Transfer::Messages::UploadBandwidthChangedMessage* m);
	
	void
	receive(Transfer::Messages::UploadCanceledMessage* m);
	
	void
	receive(Transfer::Messages::UploadCompletedMessage* m);

	void
	receive(Transfer::Messages::DownloadStartedMessage* m);
	
	void
	receive(Transfer::Messages::DownloadBandwidthChangedMessage* m);
	
	void
	receive(Transfer::Messages::DownloadCanceledMessage* m);
	
	void
	receive(Transfer::Messages::DownloadCompletedMessage* m);

	void
	sendTrackerRequest();

	void
	sendHandshake(Peer *p);
	
	void
	sendHandshakeResponse(Peer *p);
	
	void
	sendBitfield(Peer *p);
	
	
	void
	sendChoke(Peer* p);
	
	void
	sendUnchoke(Peer* p);
	
	void
	sendKeepAlive(Peer *p);

	void
	sendHave(Peer *p, ChunkID c);
	
	void
	sendChunkRequest(ChunkRequest& cr);
	
	void
	sendChunkRequestQueued(ChunkRequest& cr);
	
	void
	sendChunkRequestCanceled(ChunkRequest& cr);
	
	void
	sendQueuedChunkRequestCanceled(ChunkRequest& cr);

	void
	sendChunk(ChunkRequest& cr);
	void
	completeChunk(ChunkID c);

	void
	send(Messaging::AbstractMessage* m);

	bool
	hasChunk(ChunkID c) const;

	AutoStr 
	description() const;
	
	inline 
	bool 
	operator<(const Peer& o) const	{ return id < o.id;}
	
	inline 
	bool 
	operator>(const Peer& o) const 	{ return id > o.id;}
	
	inline 
	bool 
	operator==(const Peer& o) const	{ return id == o.id;}

	struct Sorter
	{
	    inline 
	    bool operator()(const Peer* a,
			    const Peer* b) const
	    {
		return (a->id < b->id);
	    }
	};
    };
}

std::string bitfieldToString(const std::vector<bool>& bf);

#endif // __PEER__PEER_HXX__
