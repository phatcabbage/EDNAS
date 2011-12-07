#ifndef NEWNETWORK__INTERFACES_HXX__
#define NEWNETWORK__INTERFACES_HXX__

namespace Network
{
    struct IData;
    template <typename _Protocol> struct IAddress;
    template <typename _Protocol> struct IMessage;
    template <typename _Protocol> struct INode;
    template <typename _Protocol> struct ILink;

    template <typename T>
    struct Properties
    {
//	enum { size = 1 };
    };

    struct IData
    {
	const DataType Size;
    protected:
	IData(const DataType& size) : Size(size) {}
	IData(const IData& id) : Size(id.Size) {}
	virtual ~IData() {}
    };

    template <typename _Protocol>
    struct IMessage : public IData
    {
	typedef _Protocol ProtocolType;
	typedef typename _Protocol::AddressType AddressType;
	AddressType Sender;
	AddressType Receiver;

	virtual void Sent()     = 0;
	virtual void Received() = 0;
    protected:
	IMessage(const AddressType& sender,
		 const AddressType& receiver,
		 const DataType& datasize) 
	    : IData(DataType(Properties<AddressType>::size +
			     Properties<AddressType>::size) + 
		    datasize) 
	    , Sender(sender)
	    , Receiver(receiver)
	{}
	
	IMessage(const IMessage& im) 
	    : IData(im.Size)
	    , Sender(sender)
	    , Receiver(receiver)
	{}
	virtual ~IMessage() {}
    };
    
    template <typename _Protocol>
    struct INode
    {
	typedef _Protocol ProtocolType;
	typedef typename ProtocolType::AddressType AddressType;
	typedef typename ProtocolType::MessageType MessageType;
	
	virtual void Send(AddressType& sender, 
			  AddressType& receiver,
			  IData& data)          = 0;
	virtual void Sent(MessageType& msg)     = 0;
	virtual void Received(MessageType& msg) = 0;
    };

    template <typename _Protocol>
    struct ILink
    {
	typedef _Protocol ProtocolType;
	typedef typename ProtocolType::MessageType MessageType;
	void Transmit(MessageType& msg) = 0;
    };
}

#endif // NEWNETWORK__INTERFACES_HXX__
