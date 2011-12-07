#ifndef NETWORK__MESSAGE_HXX__
#define NETWORK__MESSAGE_HXX__

namespace Network
{
    template <typename _Protocol, typename _Payload>
    struct Message : public IMessage<_Protocol>
    {
	typedef _Payload PayloadType;
	PayloadType Payload;
	
	Message(const typename IMessage<_Protocol>AddressType& sender,
		const typename IMessage<_Protocol>AddressType& receiver,
		PayloadType payload = Payload())
	    : IMessage<_Protocol>(sender,
				  receiver,
				  DataType(Properties<_Payload>::size))
	    , Payload(payload) {}
	virtual ~Message() {}
    };
}
#endif // NETWORK__MESSAGE_HXX__
