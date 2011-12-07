#ifndef SCENARIOS__BITTORRENT__PEERPROTOCOLMANAGER_HXX__
#define SCENARIOS__BITTORRENT__PEERPROTOCOLMANAGER_HXX__

#include "Protocols/BitTorrent/Interfaces.h++"
#include "./Peer.h++"

namespace Scenarios
{
    namespace BitTorrent
    {
	namespace PB = Protocols::BitTorrent;
	struct Peer::ProtocolManager
	    : public Peer::Manager
	    , public Protocols::BitTorrent::PeerInterface
	    , public Foundation::Net::Node
	{
	    ProtocolManager(Peer& owner,
			    Founation::Net::Network& net,
			    const BandwidthType& upstream,
			    const BandwidthType& downstream);
	    
	    virtual ~ProtocolManager() {}

	    void Received(PB::TrackerResponseMessage& msg);
	    void Received(PB::HandshakeMessage& msg);
	    void Received(PB::ChokeMessage& msg);
	    void Received(PB::UnchokeMessage& msg);
	    void Received(PB::InterestedMessage& msg);
	    void Received(PB::UninterestedMessage& msg);
	    void Received(PB::HaveMessage& msg);
	    void Received(PB::BitfieldMessage& msg);
	    void Received(PB::RequestMessage& msg);
	    void Received(PB::PieceMessage& msg);
	    void Received(PB::CancelMessage& msg);
	    void Received(PB::KeepAliveMessage& msg);

	    void Sent(PB::PieceMessage& msg);

	    void ContactTracker(PB::TrackerInterface* tracker);
	    void UpdateTracker(PB::TrackerInterface* tracker);
	};
	

    }
}
#endif // SCENARIOS__BITTORRENT__PEERPROTOCOLMANAGER_HXX__
