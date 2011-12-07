#ifndef __PEERMESSAGES_HXX__
#define __PEERMESSAGES_HXX__

#include "SEDNAS.h++"
#include "Transfer/Message.h++"

#include <vector>


typedef std::vector<bool> Bitfield;

namespace Peer
{
    class Peer;

    namespace Messages
    {
	struct Handshake
	{
	    static const Datasize SIZE;
	    HandshakeMessage(Peer* s, Peer* r);
	    virtual void deliver(void);
	};

	struct Choke
	{
	    static const Datasize SIZE;
	    ChokeMessage(Peer* s, Peer* r);
	    virtual void deliver(void);
	};
	  
	struct Unchoke
	{
	    static const Datasize SIZE;
	    UnchokeMessage(Peer* s, Peer* r);
	    virtual void deliver(void);
	};

	struct Interested
	{
	    static const Datasize SIZE;
	    InterestedMessage(Peer* s, Peer* r);
	    virtual void deliver(void);
	};

	struct Uninterested
	{
	    static const Datasize SIZE;
	    UninterestedMessage(Peer* s, Peer* r);
	    virtual void deliver(void);
	};

	struct Have
	{
	    static const Datasize SIZE;
	    const ChunkID chunk;
	    HaveMessage(Peer* s, Peer* r, ChunkID c);
	    virtual void deliver(void);
	};

	struct Bitfield
	{
	    static Datasize calcSize(void);
	    Bitfield* bitfield;
	    BitfieldMessage(Peer* s, Peer* r, Bitfield* bf);
	    ~BitfieldMessage();
	    virtual void deliver(void);
	};

	struct Request
	{
	    static const Datasize SIZE;
	    const ChunkID chunk;
	    RequestMessage(Peer* s, Peer* r, ChunkID c);
	    virtual void deliver(void);
	};

	struct Piece
	{
	    static Datasize calcSize(void);
	    const ChunkID chunk;
	    PieceMessage(Peer* s, Peer* r, ChunkID c);
	    virtual void deliver(void);
	};

	struct Cancel
	{
	    static const Datasize SIZE;
	    const ChunkID chunk;
	    CancelMessage(Peer* s, Peer* r, ChunkID c);
	    virtual void deliver(void);
	};

	struct KeepAlive
	{
	    static const Datasize SIZE;
	    KeepAliveMessage(Peer* s, Peer* r);
	    virtual void deliver(void);;
	};
    }
}
#endif // __PEERMESSAGES_HXX__
