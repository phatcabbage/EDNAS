#ifndef PROTOCOLS__BITTORRENT__PEERMESSAGES_HXX__
#define PROTOCOLS__BITTORRENT__PEERMESSAGES_HXX__

#include <memory>

#include "Core.h++"
#include "Foundation/Network/NetFwd.h++"

#include "./Bitmap.h++"

namespace Protocols
{
    namespace BitTorrent
    {
	struct Peer;

	struct Handshake;
	struct Choke;
	struct Unchoke;
	struct Interested;
	struct Uninterested;
	struct Have;
	struct Bitfield;
	struct Request;
	struct Piece;
	struct Cancel;
	struct KeepAlive;

	struct Handshake
	{
	    enum {SIZE = 68};
	    Handshake() {}
	    static DataType
	    Size(){ return DataType(SIZE);} 
	};

	struct Choke
	{
	    enum {SIZE = 5};
	    Choke() {}
	    static DataType
	    Size(){ return DataType(SIZE);} 

	};
	  
	struct Unchoke
	{
	    enum {SIZE = 5};
	    Unchoke() {}
	    static DataType
	    Size(){ return DataType(SIZE);} 
	};

	struct Interested
	{
	    enum {SIZE = 5};
	    Interested() {}
	    static DataType
	    Size(){ return DataType(SIZE);} 

	    static const Datasize SIZE;
	    InterestedMessage(Peer* s, Peer* r);
	    virtual void deliver(void);
	};

	struct Uninterested
	{
	    enum {SIZE = 5};
	    Uninterested() {}
	    static DataType
	    Size(){ return DataType(SIZE);} 
	};

	struct Have
	{
	    enum {SIZE = 9;}

	    const ChunkID 
	    Chunk;
	    
	    explicit
	    Have(ChunkID c) : Chunk(c) {}
	    
	    static DataType
	    Size(){ return DataType(SIZE);}
	};

	struct Bitfield
	{
	    std::auto_ptr<Bitmap> Field;
	    const DataType MsgSize;

	    explicit
	    BitfieldMessage(const Bitmap* field)
		: Field(Bitmap::Clone(field))
		, MsgSize(5 + DataType(Field->Size() / 8.0))
	    {}
	    ~BitfieldMessage(){}
	    
	    DataType
	    Size() const { return MsgSize; }
	};

	struct Request
	{
	    enum { SIZE = 17 };
	    const ChunkID Chunk;

	    explicit
	    Request(ChunkID c) : Chunk(c) {}

	    static DataType
	    Size(){ return DataType(SIZE);}
	};

	struct Piece
	{
	    enum { SIZE = 17 };
	    const ChunkID Chunk;

	    explicit
	    Request(ChunkID c) : Chunk(c) {}

	    static DataType
	    Size(){ return DataType(SIZE);}
	};

	struct Cancel
	{
	    enum { SIZE = 17 };
	    const ChunkID Chunk;

	    explicit
	    Request(ChunkID c) : Chunk(c) {}

	    static DataType
	    Size(){ return DataType(SIZE);}
	};

	struct KeepAlive
	{
	    enum {SIZE = 5};
	    KeepAlive(Peer* s, Peer* r);

	    KeepAlive() {}

	    static DataType
	    Size(){ return DataType(SIZE);}
	};

	typedef Foundation::Net::Message<Handshake>    HandshakeMessage;
	typedef Foundation::Net::Message<Choke>        ChokeMessage;
	typedef Foundation::Net::Message<Unchoke>      UnchokeMessage;
	typedef Foundation::Net::Message<Interested>   InterestedMessage;
	typedef Foundation::Net::Message<Uninterested> UninterestedMessage;
	typedef Foundation::Net::Message<Have>         HaveMessage;
	typedef Foundation::Net::Message<Bitfield>     BitfieldMessage;
	typedef Foundation::Net::Message<Request>      RequestMessage;
	typedef Foundation::Net::Message<Piece>        PieceMessage;
	typedef Foundation::Net::Message<Cancel>       CancelMessage;
	typedef Foundation::Net::Message<KeepAlive>    KeepAliveMessage;
    }
}

#endif // PROTOCOLS__BITTORRENT__PEERMESSAGES_HXX__
