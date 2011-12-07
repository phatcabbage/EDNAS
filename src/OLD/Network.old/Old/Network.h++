#ifndef NEWNETWORK__NETWORK_HXX__
#define NEWNETWORK__NETWORK_HXX__

#include <vector>

namespace Network
{
    struct INode;
    struct ILink;
    struct Network
    {
    public:
	const std::vector<INode*>&
	Nodes() const
	{ return this->Nodes_; }
	
	const std::vector<ILink*>&
	Links() const;
	{ return this->Links_; }
	
	Network()
	    : Nodes_()
	    , Links_() {}
	
	virtual ~Network() {}

    protected:
	std::vector<INode*> Nodes_;
	std::vector<ILink*> Links_;
    };
}

#endif // NEWNETWORK__NETWORK_HXX__
