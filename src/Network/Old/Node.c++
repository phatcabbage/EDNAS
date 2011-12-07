#include "./Node.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Network::Node>::
    Logger("Node");
}

namespace Network
{
    unsigned int 
    Node::
    G_ID(0);

    Node::    
    Node()
	: ID(++G_ID)
	, FrameHandlers()
	, NIC_(NULL)
    {
	Logger << "Node " << ID << " created." << Logging::endl;
    }

    void
    Node::
    Nic(NIC* nic)
    {
	if (this->NIC_) this->NIC_->OnReceived = NULL;
	this->NIC_ = nic;
	nic->OnReceived = this;
    }

    void
    Node::
    Received(Frame& f)
    {
	if (f.Receiver != this->NIC_->MAC())
	{
	    Logger << "#" << ID << " dropping packet that is addressed to " 
		   << "#" << f.Receiver << Logging::endl;
	}
	FrameHandlerMapType::iterator it =
	    FrameHandlers.find(f.Type);
	if (it == FrameHandlers.end())
	{
	    Logger << "#" << ID << " received a packet with an unhandled "
		   << " type (" << f.Type << ")" << Logging::endl;
	    return;
	}
	it->second(f);
    }
}
