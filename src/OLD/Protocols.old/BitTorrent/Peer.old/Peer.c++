#include "Peer.h++"
#include "Messaging.h++"
#include "Tracker.h++"
#include "Transfer.h++"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>

namespace Peer
{
    using namespace Messages;

    PeerID Peer::g_uidc = 0;

    Peer::
    Peer(const PeerGroup* a_pg) 
	: peerGroup(a_pg),
	  id(g_uidc++),
	  controller(new PeerController(const_cast<Peer&>(*this)))
    {
	/* Left blank. */
    }

    Peer::
    ~Peer()
    {
	delete controller;
//	delete model;
    }

    void
    Peer::
    start(void)
    {
	controller->start();
    }

    void
    Peer::
    bestowChunk(ChunkID chunk)
    {
	controller->cheatCompleteChunk(chunk);
    }

    void
    Peer::
    receive(Tracker::Messages::ResponseMessage* m)
    {
	controller->trackerResponse(m->contactList);
    }

    void
    Peer::
    receive(Messages::HandshakeMessage* m)
    {
	controller->handshake(m->sender);
    }

    void
    Peer::
    receive(Messages::HandshakeResponseMessage* m)
    {
	controller->handshakeResponse(m->sender);
    }

    void
    Peer::
    receive(Messages::BitfieldMessage* m)
    {
	controller->bitfield(m->sender, m->bitfield);
    }

    void
    Peer::
    receive(Messages::ChokeMessage* m)
    {
	controller->choked(m->sender);
    }
    
    void
    Peer::
    receive(Messages::UnchokeMessage* m)
    {
	controller->unchoked(m->sender);
    }

    void
    Peer::
    receive(Messages::KeepAliveMessage* m)
    {
	controller->keepAlive(m->sender);
    }

    void
    Peer::
    receive(Messages::HaveMessage* m)
    {
	printf("[P]\tPeer %u received a have message for Chunk %u from Peer %u.\n",
	       id, m->chunk, m->sender->id);
	
	controller->have(m->sender, m->chunk);
    }

    void
    Peer::
    receive(Messages::ChunkRequestMessage* m)
    {
	printf("[P]\tPeer %u received a request for Chunk %u from Peer %u.\n",
	       id, m->chunk, m->sender->id);

	controller->chunkRequest(m->sender, m->chunk);
    }

    void
    Peer::
    receive(Messages::ChunkRequestQueuedMessage* m)
    {
	printf("[P]\tPeer %u had its request for Chunk %u queued by Peer %u.\n",
	       id, m->chunk, m->sender->id);

	ChunkRequest cr(m->sender, m->chunk);
	controller->chunkRequestQueued(cr);
    }

    void
    Peer::
    receive(Messages::ChunkRequestCanceledMessage* m)
    {
	printf("[P]\tPeer %u had its request for Chunk %u canceled by Peer %u.\n",
	       id, m->chunk, m->sender->id);
	
	ChunkRequest cr(m->sender, m->chunk);
	controller->chunkRequestCanceled(cr);
    }

    void
    Peer::
    receive(Messages::QueuedChunkRequestCanceledMessage* m)
    {
	printf("[P]\tPeer %u had its queued request for Chunk %u canceled by Peer %u.\n",
	       id, m->chunk, m->sender->id);
	
	ChunkRequest cr(m->sender, m->chunk);
	controller->queuedChunkRequestCanceled(cr);
    }

    void
    Peer::
    receive(Transfer::Messages::UploadStartedMessage* m)
    {
	Transfer::ChunkTransferHandle* cth = m->handle;
	printf("[P]\tPeer %u started uploading Chunk %u to Peer %u.\n", 
	       id, cth->transfer->chunk, cth->transfer->receiver->id);

	controller->uploadStarted(cth);
    }

    void
    Peer::
    receive(Transfer::Messages::UploadBandwidthChangedMessage* m)
    {
	Transfer::ChunkTransferHandle *cth = m->handle;
	Bandwidth b = m->bandwidth;
	controller->uploadBandwidthChanged(cth,b);
    }

    void
    Peer::
    receive(Transfer::Messages::UploadCanceledMessage* m)
    {
	Transfer::ChunkTransferHandle *cth = m->handle;
	controller->uploadCanceled(cth);
    }

    void
    Peer::
    receive(Transfer::Messages::UploadCompletedMessage* m)
    {
	Transfer::ChunkTransferHandle *cth = m->handle;
	controller->uploadCompleted(cth);
    }

    void
    Peer::
    receive(Transfer::Messages::DownloadStartedMessage* m)
    {
	Transfer::ChunkTransferHandle* cth = m->handle;
	printf("[P]\tPeer %u started downloading Chunk %u from Peer %u.\n",
	       id, cth->transfer->chunk, cth->transfer->sender->id);

	controller->downloadStarted(cth);
    }

    void
    Peer::
    receive(Transfer::Messages::DownloadBandwidthChangedMessage* m)
    {
	Transfer::ChunkTransferHandle *cth = m->handle;
	Bandwidth b = m->bandwidth;
	controller->downloadBandwidthChanged(cth,b);
    }

    void
    Peer::
    receive(Transfer::Messages::DownloadCanceledMessage* m)
    {
	Transfer::ChunkTransferHandle *cth = m->handle;
	controller->downloadCanceled(cth);
    }

    void
    Peer::
    receive(Transfer::Messages::DownloadCompletedMessage* m)
    {
	Transfer::ChunkTransferHandle *cth = m->handle;
	controller->downloadCompleted(cth);
    }

    void
    Peer::
    sendTrackerRequest(void)
    {
	Tracker::Tracker *t = Simulator::instance()->tracker();
	Tracker::Messages::RequestMessage *m = 
	    new Tracker::Messages::RequestMessage(this, t);
	send(m);
    }

    void
    Peer::
    sendHandshake(Peer *p)
    {
	HandshakeMessage* m = new HandshakeMessage(this, p);
	send(m);
    }

    void
    Peer::
    sendHandshakeResponse(Peer *p)
    {
	HandshakeResponseMessage* m = new HandshakeResponseMessage(this, p);
	send(m);
    }

    void
    Peer::
    sendBitfield(Peer *p)
    {
	std::vector<bool>* bf = model->getChunkBitfield();
	BitfieldMessage *m = new BitfieldMessage(this, p, bf);
	send(m);
    }    

    void
    Peer::
    sendChoke(Peer* p)
    {
	ChokeMessage* m = new ChokeMessage(this, p);
	send(m);
    }

    void
    Peer::
    sendUnchoke(Peer* p)
    {
	UnchokeMessage* m = new UnchokeMessage(this, p);
	send(m);
    }

    void
    Peer::
    sendKeepAlive(Peer* p)
    {
	KeepAliveMessage* m = new KeepAliveMessage(this, p);
	send(m);
    }

    void
    Peer::
    sendHave(Peer* p, ChunkID c)
    {
	HaveMessage *m = new HaveMessage(this, p, c);
	send(m);
    }

    void 
    Peer::
    sendChunkRequest(ChunkRequest& cr)
    {
	ChunkRequestMessage* m = new ChunkRequestMessage(this, cr.peer, cr.chunk);
	send(m);
    }

    void
    Peer::
    sendChunkRequestQueued(ChunkRequest& cr)
    {
	ChunkRequestQueuedMessage* m = new ChunkRequestQueuedMessage(this, cr.peer, cr.chunk);
	send(m);
    }

    void
    Peer::
    sendChunkRequestCanceled(ChunkRequest& cr)
    {
	ChunkRequestCanceledMessage *m = new ChunkRequestCanceledMessage(this, cr.peer, cr.chunk);
	send(m);
    }

    void
    Peer::
    sendQueuedChunkRequestCanceled(ChunkRequest& cr)
    {
	QueuedChunkRequestCanceledMessage *m = 
	    new QueuedChunkRequestCanceledMessage(this, cr.peer, cr.chunk);
	send(m);
    }

    void
    sendChunk(ChunkRequest& cr)
    {
	Transfer::transfer(this, cr.peer, cr.chunk);
    }

    void
    Peer::
    send(Messaging::AbstractMessage* m)
    {
	Messaging::MessageQueue::instance()->send(m);
    }

    bool
    Peer::
    hasChunk(ChunkID c) const
    {
	/* STUB */
    }

    Bandwidth
    Peer::
    maxUpstream(void) const
    {
	return (peerGroup.maxUp());
    }

    Bandwidth
    Peer::
    maxDownstream(void) const
    {
	return (peerGroup.maxDown());
    }

    std::string
    Peer::
    description(void) const
    {
	char *resultChar;
	asprintf(&resultChar, "[(Peer) Group: %d | id: %d ]", 
		 this->peerGroup->id, this->id);

	std::string result(resultChar);
	delete resultChar;
	return result;
    }

}


/*
std::string
bitfieldToString(const std::vector<bool>& bf)
{
    std::ostringstream o;
    const static unsigned int blocksPerRow = 4;
    const static unsigned int colsPerBlock = 8;
    unsigned int row = 0;
    std::vector<bool>::const_iterator bfIt = bf.begin();
    do
    {
	o << "\t" << row++ << ":\t";
	for (unsigned int block = 0; block < blocksPerRow; block++)
	{
	    for (unsigned int col = 0; col < colsPerBlock; col++)
	    {
		if (bfIt == bf.end())
		    return o.str();
		
		if (*(bfIt++))
		    o << "I";
		else
		    o << "0";

	    }
	    o << "\t";
	}
	o << "\n";
    } while (true);
}
*/		
std::ostream&
operator<<(std::ostream& o, const Peer::Peer* p)
{
    o << "[Peer " << p->id << "]";
    return o;
}
