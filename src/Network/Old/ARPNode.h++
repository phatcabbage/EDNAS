#ifndef NETWORK__ARPNODE_HXX__
#define NETWORK__ARPNODE_HXX__

#include <map>
#include <set>

#include "Core/Signaling/Event.h++"
#include "Logging/Logger.h++"

namespace Network
{
    typedef unsigned int IPAddress;
    typedef unsigned int MACAddress;
    struct Frame;
    struct IPNode;
    struct NIC;
    struct ARPNode : public EventHandler<Frame&>,
		     public Logging::Logged<ARPNode>
    {
    public:
	ARPNode();
	~ARPNode();

	void
	Resolve(IPAddress ip);

	void operator()(Frame& f)
	{ this->Receive(Frame& f); }
	
	IPNode*
	GetIPNode() const
	{ return this->IPNode_; }

	void
	SetIPNode(IPNode* ipn)
	{ this->IPNode_ = ipn; }

	Node*
	GetNode() const
	{ return this->Node_; }

	void
	SetNode(Node* node);

    protected:
	typedef std::set<IPAddress>
	PendingRequestSet;

	PendingRequestSet
	PendingRequests_;

	IPNode*
	IPNode_;
	
	Node*
	Node_;

	void
	Receive(Frame& f);

	void
	SendRequest(IPAddress i);
    };
}

#endif // 
