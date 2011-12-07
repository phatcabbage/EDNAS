#ifndef PROTOCOLS__BITTORRENT__INTERFACES_HXX__
#define PROTOCOLS__BITTORRENT__INTERFACES_HXX__

namespace Protocols
{
    namespace BitTorrent
    {
	struct PeerInterface
	    : public TrackerResponseMessage::ReceiverType
	    , public HandshakeMessage::ReceiverType
	    , public ChokeMessage::ReceiverType
	    , public UnchokeMessage::ReceiverType
	    , public InterestedMessage::ReceiverType
	    , public UninterestedMessage::ReceiverType
	    , public HaveMessage::ReceiverType
	    , public BitfieldMessage::ReceiverType
	    , public RequestMessage::ReceiverType
	    , public PieceMessage::ReceiverType
	    , public CancelMessage::ReceiverType
	    , public KeepAliveMessage::ReceiverType
	    , public TrackerRequestMessage::SenderType
	    , public HandshakeMessage::SenderType
	    , public ChokeMessage::SenderType
	    , public UnchokeMessage::SenderType
	    , public InterestedMessage::SenderType
	    , public UninterestedMessage::SenderType
	    , public HaveMessage::SenderType
	    , public BitfieldMessage::SenderType
	    , public RequestMessage::SenderType
	    , public PieceMessage::SenderType
	    , public CancelMessage::SenderType
	    , public KeepAliveMessage::SenderType
	{
	};
    }
    
    struct TrackerInterface
	: public TrackerRequestMessage::ReceiverType
	, public TrackerResponseMessage::SenderType
    {
    };
}


#endif // PROTOCOLS__BITTORRENT__INTERFACES_HXX__
