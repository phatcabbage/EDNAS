#include <cstdlib> // random()

#include "Simulation/Timer.h++"
#include "Protocols/Ethernet.h++"
#include "LinkHandle.h++"
#include "NIC.h++"
#include "Link.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Network::Link>::
    Logger("Link");
}

namespace Network
{
    using Protocols::Ethernet;

    LinkManager
    LinkManager::Main;

    LinkManager::
    ~LinkManager()
    {
	for(auto it = Links.begin(), end = Links.end(); it != end; ++it)
	    delete *it;
    }

    TimeType
    LinkHandle::
    Delay() const
    {return Link_.Delay();}

    void
    LinkHandle::
    Received(Ethernet::PacketPtr& f)
    { f.Deliver(NIC_); }

    Link::
    Link(NIC& left,
	 NIC& right,
	 TimeType delay,
	 SizeType leftUp,
	 SizeType rightUp,
	 double reliability)
	: Handle_()
	, Queue_()
	, Delay_(delay * Time::SECOND)
	, Bandwidth_()
	, Reliability_(reliability)
    {
	Handle_[Link::Left]  = new SideLinkHandle<Link::Left>(*this, left);
	Handle_[Link::Right] = new SideLinkHandle<Link::Right>(*this, right);
	Bandwidth_[Link::Left]  = leftUp;
	Bandwidth_[Link::Right] = rightUp;
	left.Handle(Handle_[Link::Left]);
	right.Handle(Handle_[Link::Right]);
	LinkManager::Main += this;
    }
    
    template<Link::Side S>
    void
    Link::
    Send(Ethernet::PacketPtr& f)
    {
	double loss = (double)random() / (double)RAND_MAX;
	if (loss < this->Reliability_)
	{
	    Queue_[S].push_back(f);
	    new SendTimer(*this, S);
	}
    }

    Link::
    ~Link()
    {
	delete this->Handle_[Link::Left];
	delete this->Handle_[Link::Right];
    }

    template void Link::Send<Link::Left>(FramePtr& f);
    template void Link::Send<Link::Right>(FramePtr& f);
}
