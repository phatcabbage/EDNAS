#include <iostream>
#include "Core/Environment/ProgramOptions.h++"
#include "Foundation/Simulator/Simulator.h++"
#include "./Client.h++"
#include "./Server.h++"

namespace Scenarios
{
    namespace Webserver
    {
	Server::
	Server()
	    : Foundation::Net::TNode<Foundation::Net::BoundedRRMessageContainer>(
		Foundation::Simulator::instance()->Network(), 
		BandwidthType(DEF_UPSTREAM), 
		BandwidthType(DEF_DOWNSTREAM)
		)
	    , PH::ServerInterface()
	    , Resource(NULL) 
	    , OnRequestReceived()
	    , OnRequestFulfilled()
	{
	    Core::Environment::ProgramOptions* po = 
		Core::Environment::ProgramOptions::instance();
	    
	    if (po->Contains("-serverconn"))
		this->SetMaxConnections(po->Value<int>("-serverconn"));
	}
	
	uint
	Server::
	MaxConnections() const
	{ return this->Messages_.MaxActiveMessages();}
	
	void
	Server::
	SetMaxConnections(uint maxConnections)
	{ this->Messages_.SetMaxActiveMessages(maxConnections);}


	void
	Server::
	Send404(Client* const ci, const PH::URL& uri)
	{
	    PH::NotFound *nfr = new PH::NotFound(uri);
	    this->Send(new PH::NotFoundMessage(this, ci, nfr));
	}

	void
	Server::
	SendFile(Client* const ci, const PH::URL& uri)
	{
	    PH::OK *okr = new PH::OK(uri, this->Resource);
	    std::cout << "[SERVER] OKMessage is size " << okr->Size() << std::endl;
	    this->Send(new PH::OKMessage(this, ci, okr));
	}

	void
	Server::
	SendNotImplemented(Client* const ci, const PH::URL& uri)
	{
	    PH::NotImplemented *nir = new PH::NotImplemented(uri);
	    this->Send(new PH::NotImplementedMessage(this, ci, nir));
	}

	void
	Server::
	Received(Foundation::Net::IMessage* const msg)
	{
//	    Foundation::Net::Node::Received(msg);
	    std::cout << "[SERVER] Generic message received." << std::endl;
	}

	void
	Server::
	Received(PH::GetMessage* const msg)
	{
	    Client* client = 
		dynamic_cast<Client*>(msg->Sender);

	    const PH::URL& URI = msg->Payload->URI;

	    RequestArgs args(this, client, URI);
	    this->OnRequestReceived(args);
	    std::cout << "[SERVER] Received request." << std::endl;

	    if (!this->Resource)
	    {
		this->Send404(client, URI);
		return;
	    }

	    if (URI.compare(this->Resource->Name()) != 0)
	    {
		this->Send404(client, URI);
		return;
	    }

	    this->SendFile(client, URI);
	}

	void
	Server::
	Received(PH::PutMessage* const msg)
	{
	    Client* client = 
		dynamic_cast<Client*>(msg->Sender);
	    if (client)
		this->SendNotImplemented(client, msg->Payload->URI);
	}

	void
	Server::
	Received(PH::DeleteMessage* const msg)
	{
	    Client* client =
		dynamic_cast<Client*>(msg->Sender);
	    if (client)
		this->SendNotImplemented(client, msg->Payload->URI);
	}

	void
	Server::
	Sent(PH::OKMessage* const msg)
	{
	    Client* client =
		dynamic_cast<Client*>(msg->Receiver);
	    
	    const PH::URL& uri = msg->Payload->URI;

	    RequestArgs args(this, client, uri);
	}	    
    }
}
