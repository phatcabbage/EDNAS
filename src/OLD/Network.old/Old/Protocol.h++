#ifndef NETWORK__PROTOCOL_HXX__
#define NETWORK__PROTOCOL_HXX__

#include "Core/Numerical/DataTypes.h++"

namespace Network
{
    struct IMessage
    {
	const DataType Size;
	virtual void Sending() = 0;
	virtual void Sent() = 0;
	virtual void Receiving() = 0;
	virtual void Received() = 0;
	IMessage(const DataType& size)
	    : Size(size) {}
	virtual ~IMessage() {}
    };

    struct SwitchedEthernet
    {
	enum {DEF_MTU = 1500};

	typedef uint Address;

	inline const DataType&
	MTU() const { return MTU_; }
	
	struct Packet;
	
	struct Message : public IMessage
	{
	    const Address Sender;
	    const Address Receiver;

	    Message(const DataType& size,
		    const Address& sender,
		    const Address& receiver)
		: IMessage(size)
		, Sender(sender)
		, Receiver(receiver) {}
	    virtual ~Message() {}
	};
    };

    struct Node
    {
	void Enqueue(const Protocol::Address& receiver, IMessage* imsg)
	{
	    Protocol::Message* msg = new Protocol::Message(receiver, imsg);
	    bool empty = this->Queue.empty();
	    this->Queue.push_back(msg);
	    if (empty)
		this->Send();
	}
	void Send()
	{
	    Message *msg = this->Queue.front();
	    Message::Container::Message* contMsg = msg->next();
	    this->ContainerNode.Send(contMsg);
	}
	void Sent(Message::Container::Message* contMsg)
	{
	


    // template<typename _CONTAINER> 
    // struct TProtocol;

    // class IProtocol
    // {
    // 	struct Message;
    // 	struct Node;

    // 	struct Message
    // 	{
    // 	    const DataType Size;
    // 	    const Address Sender;
    // 	    const Address Receiver;
    // 	    virtual void Sending() = 0;
    // 	    virtual void Sent() = 0;
    // 	    virtual void Receiving() = 0;
    // 	    virtual void Received() = 0;
    // 	    Message(const DataType& d,
    // 		    const Address&  s,
    // 		    const Address&  r)
    // 		: DataType(d)
    // 		, Sender(s)
    // 		, Receiver(r) {}
    // 	    virtual ~Message() {}
    // 	};

	// struct Node
	// {
	// public:
	//     void 
	//     Enqueue(Message* msg) = 0;

	// protected:
	//     void
	//     Sending(Message* msg) = 0;
	    
	//     void
	//     Sent(Message* msg) = 0;
	    
	//     void
	//     Receiving(Message* msg) = 0;
	    
	//     void
	//     Received(Message* msg) = 0;
	// };
    // };


    // template<typename _CONTPROTO, typename _ADDRESSTYPE = uint>
    // struct TProtocol
    // {
    // 	typedef _CONTPROT ContainerProtocol;
    // 	typedef _ADDRESSTYPE Address;

    // 	struct Message;
    // 	struct Node;
    // };

    // template<typename _CONTPROTO, typename _ADDRESSTYPE>
    // struct TProtocol::Message
    // {
    // 	typedef _CONTPROT ContainerProtocol;
    // 	typedef _ADDRESSTYPE Address;

    // 	const Address Sender;
    // 	const Address Receiver;
    // 	const DataType Size;
	
	
    // };	

}

#endif // NETWORK__PROTOCOL_HXX__

