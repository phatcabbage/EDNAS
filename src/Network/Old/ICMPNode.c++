#include <iostream> // operator<<

#include "./IPNode.h++"
#include "./IPPacket.h++"
#include "./ICMPPacket.h++"
#include "./ICMPNode.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Network::ICMPNode>::
    Logger("ICMPNode");
}

namespace Network
{
    unsigned int
    ICMPNode::
    G_ID(0);

    ICMPNode::
    ICMPNode()
	: ID(++G_ID)
	, Node_(NULL)
    {
	Logger << *this << " created." << Logging::endl;
    }

    ICMPNode::
    ~ICMPNode()
    {
	if (this->Node_) this->Node_->PacketHandlers[IPPacket::ICMP] = NULL;
	this->Node_ = NULL;
	Logger << *this << " destroyed." << Logging::endl;
    }

    void
    ICMPNode::
    IpNode(IPNode* node)
    {
	if (this->Node_) this->Node->PacketHandlers[IPPacket::ICMP] = NULL;
	this->Node_ = node;
	if (node)
	    node->PacketHandlers[IPPacket::ICMP] = this;
    }

    void
    ICMPNode::
    SendPing(IPAddress a)
    {
	
