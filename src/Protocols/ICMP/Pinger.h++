#ifndef ICMP__PINGER_HXX__
#define ICMP__PINGER_HXX__

#include "Protocol.h++"
#include "Protocol/Node.h++"

namespace ICMP
{
    struct Pinger : public TINode<Protocol>
    {
	TSender<IPv4::Protocol>*
	Sender() const
	{ return this->Sender_; }

	virtual void 
	Sender(TSender<IPv4::Protocol>* s)
	{ 
	    this->Dissociate();
	    this->Sender_ = dynamic_cast<TIWrapperNode<IPv4::Protocol>*>(s);
	    this->Associate();
	}

	Protocol::ID
	Address() const
	{
	    if (this->Sender_)
		return this->Sender_->Address();
	    else
		return IPv4::Protocol::INVALID_ADDRESS;
	}

	virtual void
	Send(Protocol::PacketPtr& pkt);
// 	{
// 	    if (this->Sender_)
// 	    {
// 		pkt->Sender = this->Address();
// 		auto ippkt = 

	void SendPing(IPv4::AddressType ip);
	void ReceivedPing(IPv4::AddressType ip);
	void SendPong(IPv4::AddressType ip);
	void ReceivedPong(IPv4::AddressType ip);

	Pinger(IPNode& node);
	~Pinger();

    protected:
	TIWrapperNode<IPv4::Protocol>*
	Sender_;
	
	void 
	Associate()
	{ if (this->Sender_) this->Sender_->Bind(Protocol::TypeCode, this); }

	void
	Dissociate()
	{ if (this->Sender_) this->Sender_->Unbind(Protocol::TypeCode); }
    };
}

#endif // NODES__PINGER_HXX__
