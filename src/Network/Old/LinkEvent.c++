#include "Simulation/Timer.h++"
#include "./LinkHandle.h++" 
#include "./LinkEvent.h++"

namespace Network
{
    FrameSent::
    FrameSent(LinkHandle& h, FramePtr& f)
	: LinkEvent(h,f)
    {
	TimeType time = Frame_->Length / Handle_.Upstream();
	Simulation::Timer::Main.Schedule(time, *this);
    }	

    void
    FrameSent::
    operator()()
    {
	this->Handle_.Sent(this->Frame_);
	delete this;
    }

    FrameReceived::
    FrameReceived(LinkHandle& h, FramePtr& f)
	: LinkEvent(h,f)
    {
	double time = Handle_.Delay() + (Frame_->Length / Handle_.Upstream());
	Simulation::Timer::Main.Schedule(time, *this);
    }

    void
    FrameReceived::
    operator()()
    {
	this->Handle_.Received(this->Frame_);
	delete this;
    }
}
