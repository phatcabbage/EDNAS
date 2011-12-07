#ifndef NETWORK__NODE_HXX__
#define NETWORK__NODE_HXX__

#include <map>
#include <queue>

#include "Core/Signaling/SingleEvent.h++"
#include "Logging/Logged.h++"
#include "./Frame.h++"
#include "./NIC.h++"

namespace Network
{
    struct Node : public EventHandler<Frame&>, 
		  public Logging::Logged<Node>
    {
	typedef EventHandler<Frame&> Handler;

	typedef std::map<Ethernet::SubType, SingleEvent<Frame&> >
	FrameHandlerMapType;

	const unsigned int ID;

	FrameHandlerMapType
	FrameHandlers;

	Node();
	~Node() {}
	
	NIC*
	Nic() const
	{ return this->NIC_; }
	
	MACAddress
	MAC() const
	{ return this->NIC_ ? this->NIC_->MAC() : 0;}

	void
	Nic(NIC* nic);

	void
	Send(Frame* frm);

    protected:
	static unsigned int G_ID;
	
	NIC*
	NIC_;
	
	void
	Received(Frame& f);

	void
	operator()(Frame& f)
	{ this->Received(f); }
    };
}

#endif // NETWORK__NODE_HXX__
