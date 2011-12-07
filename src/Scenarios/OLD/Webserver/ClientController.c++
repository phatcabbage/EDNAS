#include <iostream>
#include "./Client.h++"
#include "./ClientFactory.h++"
#include "./Scenario.h++"
#include "./ClientController.h++"

namespace Scenarios
{
    namespace Webserver
    {
	ClientController::
	ClientController() 
	    : OutstandingClients(0)
	{
	}

	void
	ClientController::
	Init()
	{
	    Scenario* scen = Scenario::instance();
	    Server* server = &(scen->WebServer);
	    const std::vector<Client*> clients =
		scen->Source.Clients();
	    
	    uint count = clients.size();
	    for (this->OutstandingClients = 0; this->OutstandingClients < count; ++(this->OutstandingClients))
		clients[this->OutstandingClients]->Request(server, &(scen->File));
	}
	
	void
	ClientController::
	ClientFileRequestSent(Client* c)
	{
	    std::cout << "[CC] Client file request sent." << std::endl;
	}
	    
	void
	ClientController::
	ClientFileReceived(Client* c)
	{
	    std::cout << "[CC] Client received file." << std::endl;
	    if (--(this->OutstandingClients) == 0) Foundation::Simulator::instance()->Stop();

	}
    }
}
