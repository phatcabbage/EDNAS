#include "./Node.h++"

namespace Scenarios
{
    namespace NameServ
    {
	Client::
	Client(Net::Network* net,
	       PD::ServerInterface* dnsServer)
	    : Net::Node(net, BandwidthType(1048576), BandwidthType(1048576))
	    , PP::PingInterface()
	    , PD::QuerierInterface()
	    , PH::ClientInterface()
	    , DNSServer_(dnsServer)
	    , Peers_()
	    , UnconfirmedPeers_()
	    , OnAddNewPeer() {}

	void
	Client::
	Received(PP::PongMessage* const msg)
	{
	    PP::PingInterface* sender = 
		dynamic_cast<PP::PingInterface*>(msg->Sender);

	    if (!sender)
		return;

	    if (!UnconfirmedPeers_.count(sender))
		return;

	    UnconfirmedPeers_.erase(sender);
	    Peers_.insert(sender);
	}

	void
	

	
