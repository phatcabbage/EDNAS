//#include "./IPPacket.h++"
#include "./Frame.h++"
#include "./IPNode.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Network::IPNode>::
    Logger("IPNode");
}

namespace Network
{
    unsigned int
    IPNode::
    G_IP(0);

    IPNode::
    IPNode()
	: IP(++G_IP)
	, PacketHandlers()
	, MACByIP()
	, IPByMAC()
	, Queue_()
	, AwaitingARP()
    {
	Logger << "#" << IP << " created." << Logging::endl;
    }
    
    void
    IPNode::
    EtherNode(Node* node)
    {
	if (this->Node_) this->Node_->FrameHandlers[Ethernet::IPv4] = NULL;
	this->Node_ = node;
	if (node)
	    node->FrameHandlers[Ethernet::IPv4] = this;
    }

    void
    IPNode::
    Send(IPPacket* pkt)
    {
	if (!this->Node_) return;
	if (this->MACByIP.count(pkt->Receiver))
	{
	    MACAddress& mac = this->MACByIP[pkt->Receiver];
	    Frame f(Ethernet::IPv4, this->Node_.Nic()->Mac(),
		    mac, pkt->Size);
	    this->Node_->Send(&f);
	    return;
	}
	else
	{
	    IPPacket* arpPkt = 
		new IPPacket(this->IP, pkt->Receiver,
			     0, IPPacket::ARP, NULL);
					    
	    Frame f(this->Node_->NIC()->MAC(), 
		    0,
		    arpPkt.Size, Frame::IPv4, arpPkt);
	    this->AwaitingARP.push_back(pkt);
	    this->Node_->Send(&f);
	}
    }

    void
    IPNode::
    Received(Frame& f)
    {
	if (f.Receiver != this->NIC_)
	{
	    Logger << *this << " dropping frame belonging to someone else..."
		   << Logging::endl;
	    return;
	}
	IPPacket* pkt = dynamic_cast<IPPacket*>(f.Message->Payload);
	if (!pkt)
	{
	    Logger << *this << " dropping erroneous ip packet."
		   << Logging::endl;
	    return;
	}


	// ARP
	if (f->Sender != 0 && pkt->Sender != 0)
	{
	    if (MACByIP.count(pkt->Sender) == 0)
	    {
		Logger << *this << " Updating ARP tables." << Logging::endl;
		MACByIP[pkt->Sender] = f->Sender;
		QueueType::iterator it = AwaitingMAC_.begin(),
		    end = AwaitingMAC_.end();
		while(it != end)
		{
		    if ((*it)->Receiver == pkt->Sender)
		    {
			this->Send(*it);
			AwaitingMAC_.erase(it++);
		    }
		    else
			++it;
		}
	    }
	}		    

	// Wrong IP
	if (pkt->Receiver != this->IP)
	{
	    Logger << *this << " dropping IP packet addressed to someone "
		   << "else." << Logger::endl;
	}

	PacketHandlerMapType::iterator it =
	    PacketHandlers.find(pkt->Type);
	
	if (it == PacketHandlers.end())
	{
	    Logger << *this << " dropping IP packet type for which "
		   << "no handler is assigned." << Logging::endl;
	    return;
	}
	it->second(*pkt);
    }
}
