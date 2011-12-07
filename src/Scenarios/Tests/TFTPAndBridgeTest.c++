#include <string>

#include "SEDNAS.h++"

#include "Simulation/Timer.h++"
#include "Core/Signaling/EventAdapter.h++"
#include "Logging/OStreamLogger.h++"

#include "Nodes/Bridge.h++"
#include "Nodes/TFTPServer.h++"
#include "Nodes/TFTPClient.h++"

Logging::OStreamLogger
SimLog("TFTP Test");

static const TimeType delay = 2 * Time::MILLISECOND;	// 2 ms delay on link
static const SizeType bw = 64 * Bandwidth::KILOBIT;	// 64 kbit bandwidth

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
	SimLog << "Upload Completed in " << delta << " sec " 
	       << Logging::endl << "(" << (args.Size / delta) 
	       << " (bit/s)" << Logging::endl;
	
	Start_ = Simulation::Timer::Main.Time();
	Flag_ = Downloading;
	Requestor* rq = new Requestor;
	*rq = { this, args.FileName };
	Simulation::Timer::Main.Schedule(0, *rq);
    }

    void
    CompletedDownload(Nodes::TFTPClient::TransferEventArgs& args)
    {
	TimeType delta = Simulation::Timer::Main.Time() - Start_;
	SimLog << "Download Completed in " << delta << " sec " 
	       << Logging::endl << "(" << (args.Size / delta) 
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
    Logging::Logged<Simulation::Timer>::Verbosity(Logging::Warning);

    Server server;
    Client client1;
    Client client2;
    client1.TFTPNode_.Server(server.IPNode_.Address());
    client2.TFTPNode_.Server(server.IPNode_.Address());

    auto s1 = new Sendor, s2 = new Sendor;
    *s1 = { &client1, "test file 1", 1048576 * Size::BYTE};
    *s2 = { &client2, "test file 2", 1048576 * Size::BYTE};
    Simulation::Timer::Main.Schedule(0, *s1);
    Simulation::Timer::Main.Schedule(0, *s2);
    
    Nodes::Bridge bridge;
    bridge += server.EthernetNode_;
    bridge += client1.EthernetNode_;
    bridge += client2.EthernetNode_;

    Simulation::Timer::Main.Run();
    
    return 0;
}
