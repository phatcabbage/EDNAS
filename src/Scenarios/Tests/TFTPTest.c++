#include <string>
#include <ctime>

#include "Simulation/Timer.h++"
#include "Core/Signaling/EventAdapter.h++"
#include "Logging/OStreamLogger.h++"

#include "Nodes/TFTPServer.h++"
#include "Nodes/TFTPClient.h++"

#include "Link/LossyPointToPoint.h++"

Logging::OStreamLogger
SimLog("TFTP Test");

static const TimeType	delay	= 2 * Time::MILLISECOND;
static const SizeType	bw	= 64 * Bandwidth::KILOBIT;
static const double	rel	= 1;		// 25% chance of losing packets

Protocols::IPv4::AddressType 
G_IP = 0;

struct Client;

struct Sendor
{
    Client* Client_;
    std::string FileName_;
    SizeType Size;
    void operator()();
};

struct Requestor
{
    Client* Client_;
    std::string FileName_;
    void operator()();
};

struct Server
{
    Nodes::EthernetNode	EthernetNode_;
    Nodes::IPNode	IPNode_;
    Nodes::UDPNode	UDPNode_;
    Nodes::TFTPServer	TFTPNode_;

    Server() : EthernetNode_()
	     , IPNode_(EthernetNode_, ++G_IP)
	     , UDPNode_(IPNode_)
	     , TFTPNode_(UDPNode_, false)
    {
	SimLog << "Server Created:" << Logging::endl;
	SimLog << "\tMAC: " << (EthernetNode_.Address()) << Logging::endl;
	SimLog << "\tIP:  " << (IPNode_.Address()) << Logging::endl;
    }
};

struct Client
{
    enum StatusFlag { Uploading, Downloading };
    Nodes::EthernetNode EthernetNode_;
    Nodes::IPNode	IPNode_;
    Nodes::UDPNode	UDPNode_;
    Nodes::TFTPClient	TFTPNode_;
    StatusFlag		Flag_;

    EventAdapter<Client, Nodes::TFTPClient::TransferEventArgs&>
    TransferEventAdapter_;

    TimeType Start_;

    Client() : EthernetNode_()
	     , IPNode_(EthernetNode_, ++G_IP)
	     , UDPNode_(IPNode_)
	     , TFTPNode_(UDPNode_)
	     , Flag_(Uploading)
	     , TransferEventAdapter_(*this, &Client::OnTransferEvent)
	     , Start_(0)
    {
	this->TFTPNode_.OnTransferEvent += this->TransferEventAdapter_;
	SimLog << "Client Created:" << Logging::endl;
	SimLog << "\tMAC: " << (EthernetNode_.Address()) << Logging::endl;
	SimLog << "\tIP:  " << (IPNode_.Address()) << Logging::endl;
    }

    void 
    CompletedUpload(Nodes::TFTPClient::TransferEventArgs& args)
    {
	TimeType delta = Simulation::Timer::Main.Time() - Start_;
	SimLog << "Upload Completed in " << (double)delta / 1000 << " msec " 
	       << Logging::endl << "(" << ((double)args.Size * 1000000000/ delta) 
	       << " (bit/s)" << Logging::endl;
	
	Start_ = Simulation::Timer::Main.Time();
	Flag_ = Downloading;
	Requestor* rq = new Requestor;
	*rq = { this, args.FileName };
	Simulation::Timer::Main.Schedule(1, *rq);
    }

    void
    CompletedDownload(Nodes::TFTPClient::TransferEventArgs& args)
    {
	TimeType delta = Simulation::Timer::Main.Time() - Start_;
	SimLog << "Download Completed in " << (double)delta / 1000 << " msec " 
	       << Logging::endl << "(" << ((double)args.Size * 1000000000/ delta) 
	       << " (bit/s)" << Logging::endl;
    }

    void
    OnTransferEvent(Nodes::TFTPClient::TransferEventArgs& args)
    {
	if (args.Type == Nodes::TFTPClient::TransferEventType::Completed)
	{
	    if (this->Flag_ == Uploading)
		this->CompletedUpload(args);
	    else
		this->CompletedDownload(args);
	}
	else
	{
	    if (this->Flag_ == Uploading)
		SimLog << "Upload: ";
	    else
		SimLog << "Download: ";

	    switch(args.Type)
	    {
	    case (Nodes::TFTPClient::TransferEventType::Completed):
		SimLog << "Completed.";
		break;
	    case (Nodes::TFTPClient::TransferEventType::TimedOut):
		SimLog << "Timed out.";
		break;
	    case (Nodes::TFTPClient::TransferEventType::Error):
		SimLog << "Error.";
		break;
	    };

	    SimLog << Logging::endl;
	}
    }
};    

void
Sendor::
operator()()
{ Client_->TFTPNode_.SendFile(FileName_, Size); delete this; }

void
Requestor::
operator()()
{ Client_->TFTPNode_.RequestFile(FileName_); delete this; }

int main(int argc, char** argv)
{
    const std::string TestFileName("Test file");
    
    Logging::Logged<Simulation::Timer>::Verbosity(Logging::Warning);

    srandom(time(NULL));

    Server server;
    Client client;

    Links::LossyPointToPointLink<Protocols::Ethernet>
	link(rel, delay, bw, bw);

    link.Connect(server.EthernetNode_, client.EthernetNode_);

    client.TFTPNode_.Server(server.IPNode_.Address());

    auto s = new Sendor;
    *s = { &client, TestFileName, 2048 * Size::BYTE};

    Simulation::Timer::Main.Schedule(0, *s);
    
    Simulation::Timer::Main.Run();
    
    return 0;
}
