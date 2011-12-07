#ifndef SCENARIOS__BITTORRENT__PEERINFO_HXX__
#define SCENARIOS__BITTORRENT__PEERINFO_HXX__

#include <memory>

#include "./Peer.h++"

namespace Scenarios
{
    namespace BitTorrent
    {
	struct Peer::PeerInfo
	{
	    enum PeerStatus
	    {
		NONE   = 0b0000,
		ISCHOKED = 0b0001,
		ISINTERESTED = 0b0010,
		AMCHOKED = 0b0100,
		AMCHOKING = 0b1000,
		DEFAULT = (ISCHOKED | AMCHOKING)
	    };

	    PeerInterface* const
	    Address;

	    std::auto_ptr<Bitmap>
	    ChunkMap;
	    
	    PeerStatus
	    Status;

	    int
	    InterestingChunks;
	    
	    PeerInfo(PeerInterface* const address,
		     const Bitmap* const chunkMap = NULL)
		: Address(address)
		, ChunkMap(chunkMap)
		, Status(DEFAULT)
		, InterestingChunks(0)
	    {
		CalculateInterestingChunks();
	    }

	    void
	    CalculateInterestingChunks();
	
	    inline bool
	    IsChoked() const 
	    { return Status & ISCHOKED; }

	    inline bool
	    IsInterested() const 
	    { return Status & ISINTERESTED; }

	    inline bool
	    AmChoked() const 
	    { return Status & AMCHOKED; }

	    inline bool
	    AmInterested() const 
	    { return Status & AMINTERESTED; }

	    inline void
	    SetIsChoked(bool val)
	    { 
		if (val) Status |= ISCHOKED;
		else Status &= ~ISCHOKED;
	    }

	    inline void
	    SetIsInterested(bool val)
	    {
		if (val) Status |= ISINTERESTED;
		else Status &= ~ISINTERESTED;
	    }
	    inline void
	    SetAmChoked(bool val)
	    { 
		if (val) Status |= AMCHOKED;
		else Status &= ~AMCHOKED;
	    }

	    inline void
	    SetAmInterested(bool val)
	    {
		if (val) Status |= AMINTERESTED;
		else Status &= ~AMINTERESTED;
	    }
	};
    }
}
 
#endif // SCENARIOS__BITTORRENT__PEERINFO_HXX__
