#include "PeerMessages.h++"
#include "PeerBase.h++"
#include "../Transfer/Message.h++"
#include "../Simulator.h++"
#include "../File.h++"

namespace Peer
{
    namespace Messages
    {
	// Handshake Message

	const Datasize HandshakeMessage::SIZE = 68;

	HandshakeMessage::
	HandshakeMessage(Peer* const s, Peer* const r) 
	    : Transfer::Message(SIZE,s,r)
	{
	    /* This function intentionally left blank. */
	}

	void 
	HandshakeMessage::
	deliver(void)
	{
	    receiver->receive(this);
	}


	// Handshake Response Message

	const Datasize HandshakeResponseMessage::SIZE = 68;

	HandshakeResponseMessage::
	HandshakeResponseMessage(Peer* const s, Peer* const r) 
	    : Transfer::Message(SIZE,s,r)
	{
	    /* Left blank. */
	}

	void
	HandshakeResponseMessage::
	deliver(void)
	{
	    receiver->receive(this);
	}


	// Choke Message

	const Datasize ChokeMessage::SIZE = 5;

	ChokeMessage::
	ChokeMessage(Peer* const s, Peer* const r)
	    : Transfer::Message(SIZE,s,r)
	{
	    /* Left blank. */
	}

	void
	ChokeMessage::
	deliver(void)
	{
	    receiver->receive(this);
	}
	

	// Unchoke Message

	const Datasize UnchokeMessage::SIZE = 5;

	UnchokeMessage::
	UnchokeMessage(Peer* const s, Peer* const r)
	    : Transfer::Message(SIZE,s,r)
	{
	    /* Left blank. */
	}

	void
	UnchokeMessage::
	deliver(void)
	{
	    receiver->receive(this);
	}


	// Interested Message

	const Datasize InterestedMessage::SIZE = 5;

	InterestedMessage::
	InterestedMessage(Peer* const s, Peer* const r)
	    : Transfer::Message(SIZE,s,r)
	{
	    /* Left blank. */
	}

	void
	InterestedMessage::
	deliver(void)
	{
	    receiver->receive(this);
	}


	// Uninterested Message

	const Datasize UninterestedMessage::SIZE = 5;

	UninterestedMessage::
	UninterestedMessage(Peer* const s, Peer* const r)
	    : Transfer::Message(SIZE,s,r)
	{
	    /* Left blank. */
	}
	
	void
	UninterestedMessage::
	deliver(void)
	{
	    receiver->receive(this);
	}

	// Have Message

	const Datasize HaveMessage::SIZE = 9;

	HaveMessage::
	HaveMessage(Peer* const s, Peer* const r, ChunkID c) 
	    : Transfer::Message(SIZE,s,r)
	{
	    /* Left blank. */
	}

	void
	HaveMessage::deliver(void)
	{
	    receiver->receive(this);
	}


	// Bitfield Message

	Datasize
	BitfieldMessage::
	calcSize(void)
	{
	    Datasize size = 5;
	    Datasize chunkCount = Simulator::instance()->file()->chunkCount();
	    size += (chunkCount / 8);
	    return size;
	}

	BitfieldMessage::
	BitfieldMessage(Peer* const s, Peer* const r, Bitfield* bf) 
	    : Transfer::Message(calcSize(),s,r),
	      bitfield(bf)
	{
	    /* Left blank. */
	}

	BitfieldMessage::
	~BitfieldMessage()
	{
	    if (bitfield) delete bitfield;
	}

	void
	BitfieldMessage::
	deliver(void)
	{
	    receiver->receive(this);
	}


	// Request Message

	const Datasize RequestMessage::SIZE = 17;

	RequestMessage::
	RequestMessage(Peer* s, Peer* r, ChunkID c)
	    : Transfer::Message(SIZE,s,r),
	      chunk(c)
	{
	    /* Left blank. */
	}

	void
	RequestMessage::
	deliver(void)
	{
	    receiver->receive(this);
	}


	// Piece Message
	
	Datasize
	PieceMessage::
	calcSize(void)
	{
	    Datasize size = 5;
	    Datasize payloadSize = Simulator::instance()->file()->chunkSize();
	    size += payloadSize;
	    return size;
	}

	PieceMessage::
	PieceMessage(Peer* s, Peer* r, ChunkID c)
	    : Transfer::Message(calcSize,s,r),
	      chunk(c)
	{
	    /* Left blank. */
	}
	
	void
	PieceMessage::
	deliver(void)
	{
	    receiver->receive(this);
	}

	// Cancel Message
	
	const Datasize CancelMessage::SIZE = 17;

	CancelMessage::
	CancelMessage(Peer* s, Peer* r, ChunkID c)
	    : Transfer::Message(SIZE,s,r),
	      chunk(c)
	{
	    /* Left blank. */
	}

	void
	CancelMessage::
	deliver(void)
	{
	    receiver->receive(this);
	}
	

	// Keep Alive Message

	const Datasize KeepAliveMessage::SIZE = 4;

	KeepAliveMessage::
	KeepAliveMessage(Peer* const s, Peer* const r)
	    : Transfer::Message(SIZE,s,r)
	{
	    /* Left blank. */
	}

	void
	KeepAliveMessage::
	deliver(void)
	{
	    receiver->receive(this);
	}
    }
}
