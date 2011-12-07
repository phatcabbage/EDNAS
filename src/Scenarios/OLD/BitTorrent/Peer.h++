#ifndef SCENARIOS__BITTORRENT__PEER_HXX__
#define SCENARIOS__BITTORRENT__PEER_HXX__

#include <map>
#include <memory>

namespace Scenarios
{
    namespace BitTorrent
    {
	using Protocols::BitTorrent::PeerInterface;
	struct Peer
	{
	    struct Manager;
	    struct ProtocolManager;
	    struct PeeringManager;
	    struct DownloadManager;
	    struct UploadManager;

	    struct PeerInfo;
	    
	    typedef std::map<PeerInfo*> PeerInfoContainer;

	    PeerInfoContainer
	    PeerInfos;
	    
	    void
	    ChokedByPeer(PeerInterface* p);

	    void 
	    UnchokedByPeer(PeerInterface* p);

	    void
	    InterestingToPeer(PeerInterface* p);

	    void
	    UninterestingToPeer(PeerInterface* p);
	};
	
	struct Peer::Manager
	{
	    Peer& 
	    Owner;

	    Manager(Peer& owner) : Owner(owner) {}
	    virtual ~Manager() {}

	    virtual void Init()  {}
	    virtual void Start() {}
	    virtual void Stop()  {}
	    virtual void Deinit(){}
	};

    } // BitTorrent
} // Scenarios
