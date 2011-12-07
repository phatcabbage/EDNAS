#ifndef __ARCHITECTURES__BITTORRENT__BITTORRENT_HXX__
#define __ARCHITECTURES__BITTORRENT__BITTORRENT_HXX__

#include "SEDNAS.h++"

template<typename Class, typename IntType>
class ID;

namespace Transfer
{
    template<typename Sender,typename Receiver, typename Payload>
    class Message;
}

namespace Architectures
{
    namespace BitTorrent
    {
	class IPeer;
	class ITracker;
	
	class Peer;
	class Tracker;

	class TorrentFile;
	class Chunk;

	class TrackerRequest;
	class TrackerResponse;

	class Handshake;
	class Choke;
	class Unchoke;
	class Interested;
	class Uninterested;
	class Have;
	class Bitfield;
	class Request;
	class Piece;
	class Cancel;
	class KeepAlive;

	typedef Transfer::Message<IPeer,ITracker,TrackerRequest>  TrackerRequestMessage;
	typedef Transfer::Message<ITracker,IPeer,TrackerResponse> TrackerResponseMessage;

	typedef Transfer::Message<IPeer,IPeer,Handshake>    HandshakeMessage;
	typedef Transfer::Message<IPeer,IPeer,Choke>        ChokeMessage;
	typedef Transfer::Message<IPeer,IPeer,Unchoke>      UnchokeMessage;
	typedef Transfer::Message<IPeer,IPeer,Interested>   InterestedMessage;
	typedef Transfer::Message<IPeer,IPeer,Uninterested> UninterestedMessage;
	typedef Transfer::Message<IPeer,IPeer,Have>         HaveMessage;
	typedef Transfer::Message<IPeer,IPeer,Bitfield>     BitfieldMessage;
	typedef Transfer::Message<IPeer,IPeer,Request>      RequestMessage;
	typedef Transfer::Message<IPeer,IPeer,Piece>        PieceMessage;
	typedef Transfer::Message<IPeer,IPeer,Cancel>       CancelMessage;
	typedef Transfer::Message<IPeer,IPeer,KeepAlive>    KeepAliveMessage;

	typedef ID<Peer,uint>        PeerID;
	typedef ID<TorrentFile,uint> FileID;
	typedef ID<Chunk,uint>       ChunkID;
    }
}


#endif // __ARCHITECTURES__BITTORRENT__BITTORRENT_HXX__
