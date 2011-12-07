#include "Foundation/Timing/Clock.h++"
#include "Foundation/Simulator/Simulator.h++"
#include "./Server.h++"
#include "./Scenario.h++"
#include "./Client.h++"

namespace Scenarios
{
    namespace Webserver
    {
	Client::
	Client(Foundation::Net::Network* net, 
	       const BandwidthType& up,
	       const BandwidthType& down)
	    : Foundation::Net::Node(net, up, down)
	    , PH::ClientInterface()
	    , RequestSentTime(0)
	    , Waits(0)
	{}

	void
	Client::
	SendGet(Server* si, const PH::URL& uri)
	{
	    PH::Get* gr = new PH::Get(uri);
	    this->Send(new PH::GetMessage(this, si, gr));
	}

	void
	Client::
	Request(Server* si, const PH::File* const file)
	{
	    this->SendGet(si, file->Name());
	}

	void
	Client::
	Received(PH::OKMessage* const msg)
	{
	    std::cout << "[HC] Client successfully downloaded a file!" << std::endl;
	    TimeType now = Foundation::Simulator::instance()->Clock()->Time();
	    TimeType wait = now - this->RequestSentTime;
	    this->Waits.push_back(wait);
	    Scenario::instance()->Controller.ClientFileReceived(this);
//	    this->OnFileReceived(*this);
	}

	void
	Client::
	Received(PH::NotFoundMessage* const msg)
	{
	    std::cout << "[HC] Client was just notified that the server can't find the file!" << std::endl;
	}

	void
	Client::
	Received(PH::NotImplementedMessage* const msg)
	{
	    std::cout << "[HC] Client was just informed that the server can't do everything it wants." << std::endl;
	}

	void
	Client::
	Sent(PH::GetMessage* const msg)
	{
	    this->RequestSentTime 
		= Foundation::Simulator::instance()->Clock()->Time();
	    Scenario::instance()->Controller.ClientFileRequestSent(this);
	}
    }
}
