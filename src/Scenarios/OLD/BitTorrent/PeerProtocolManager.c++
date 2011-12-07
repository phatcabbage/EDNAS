#include "./PeerProtocolManager.h++"

namespace Scenarios
{
    namespace BitTorrent
    {
	Peer::ProtocolManager::
	ProtocolManager(Peer& owner,
			Founation::Net::Network& net,
			const BandwidthType& upstream,
			const BandwidthType& downstream)
	    : Peer::Manager(owner)
	    , Protocols::BitTorrent::PeerInterface()
	    , Foundation::Net::Node(net, upstream, downstream)
	{
	}

	void
	Peer::ProtocolManager::
	Received(PB::TrackerResponseMessage& msg)
	{
	    Owner.TrackerResponded(msg->Sender, msg->Payload);
	}

	void 
	Peer::ProtocolManager::
	Received(PB::HandshakeMessage& msg)
	{
	    Owner.HandshakeReceived(msg->Sender);
	}
	
	void
	Peer::ProtocolManager::
	Received(PB::ChokeMessage& msg)
	{
	    Owner.ChokedByPeer(msg->Sender);
	}
	
	void 
	Peer::ProtocolManager::
	Received(PB::UnchokeMessage& msg)
	{
	    Owner.UnchokedByPeer(msg->Sender);
	}
	
	void 
	Peer::ProtocolManager::
	Received(PB::InterestedMessage& msg)
	{
	    Owner.InterestingToPeer(msg->Sender);
	}

	void 
	Peer::ProtocolManager:: 
	Received(PB::UninterestedMessage& msg)
	{
	    Owner.UninterestingToPeer(msg->Sender);
	}

	void 
	Peer::ProtocolManager:: 
	Received(PB::HaveMessage& msg)
	{
	    Owner.PeerHasChunk(msg->Sender, msg->Payload->Chunk);
	}

	void
	Peer::ProtocolManager::
	Received(PB::BitfieldMessage& msg)
	{
	    Owner.UpdatePeerBitfield(msg->Sender, msg->Payload->Field);
	}

	void
	Peer::ProtocolManager::
	Received(PB::RequestMessage& msg)
	{
	    Owner.PeerRequestedChunk(msg->Sender, msg->Payload->Chunk);
	}

	void
	Peer::ProtocolManager::
	Received(PB::PieceMessage& msg)
	{
	    Owner.ReceivedChunk(msg->Sender, msg->Payload->Chunk);
	}

	void
	Peer::ProtocolManager::
	Received(PB::CancelMessage& msg)
	{
	    Owner.PeerCanceledRequest(msg->Sender, msg->Payload->Chunk);
	}

	void
	Peer::ProtocolManager::
	Received(PB::KeepAliveMessage& msg)
	{}

	void
	Peer::ProtocolManager::
	Sent(PB::PieceMessage& msg)
	{
	    Owner.SentChunkToPeer(msg->Receiver, msg->Payload->Chunk);
	}

	void
	Peer::ProtocolManager::
	ContactTracker(PB::TrackerInterface* tracker)
	{
	    this->Enqueue(new PB::TrackerRequestMessage(this, tracker));
	}

	void
	Peer::ProtocolManager::
	UpdateTracker(PB::TrackerInterface* tracker)
	{
	    this->Enqueue(new PB::TrackerRequestMessage(this, tracker));
	}
    }
}
