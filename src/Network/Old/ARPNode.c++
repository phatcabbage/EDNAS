#include "./ARPNode.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Network::ARPNode>::
    Logger("ARPNode");
}

namespace Network
{
    ARPNode::
    ARPNode()
	: PendingRequests_()
	, IPNode_(NULL)
	, Node_(NULL)
    {
	Logger << "Created." << Logging::endl;
    }

    ARPNode::
    ~ARPNode()
    {}

    MACAddress
    ARPNode:
    Resolve(IPAddress ip)
    {
	this->SendRequest(ip);
	this->PendingRequests_.insert(ip);
    }

    void
    ARPNode::
    SetIPNode(IPNode* ipn)
    { 
	this->PendingRequests_.clear();
	// if (this->IPNode_)
	//     this->MACByIP_.erase(this->IPNode_);
	
	this->IPNode_ = ipn;
	// if (this->IPNode_ && this->Node_)
	//     this->MACByIP_[ipn->IP] = this->Node_.MAC();
    }

    void
    ARPNode::
    SetNode(Node* node)
    { 
	this->PendingRequests_.clear();
	if (this->Node_) 
	    this->Node_->FrameHandlers.erase(Frame::ARP);
	this->Node_ = node;
	if (node)
	    node->FrameHandlers[Frame::ARP] = this;
	// if (this->IPNode_ && this->Node_)
	//     this->MACByIP_[this->IPNode_->IP] = this->Node_.MAC();
    }
}
