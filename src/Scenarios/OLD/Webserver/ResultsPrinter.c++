#include <fstream>
#include <iostream>
#include <string>

#include "Core/Environment/ProgramOptions.h++"
#include "./Client.h++"
#include "./Scenario.h++"
#include "./ResultsPrinter.h++"

namespace Scenarios
{
    namespace Webserver
    {
	using Core::Environment::ProgramOptions;

	void
	ResultsPrinter::
	operator()() const
	{
	    this->Description();
	    this->ClientTimes();
	}

	void
	ResultsPrinter::
	Description() const
	{
	    ProgramOptions* po = ProgramOptions::instance();
	    if (!po->Contains("-fdesc"))
		return;
	    std::string fileName = po->Value<string>("-fdesc");
	    std::ofstream out(fileName.c_str());
	    if (!out)
	    {
		std::cerr << "[RP] ERROR: Unable to open \"" << 
		    fileName << "\" for output." << std::endl;
		return;
	    }

	    Scenario* scen = Scenario::instance();
	    out << "File: " << scen->File << "\n";
	    out << "Server: " << scen->WebServer.Upstream 
		<< " up, " << scen->WebServer.Downstream << " down" 
		<< "\n";
	    
	    const std::vector<Client*>& clients = scen->Source.Clients();
	    out << "Clients: " << clients.size();
	    if (clients.size() > 0)
		out << ", " << clients[0]->Upstream << " up, "
		    << clients[0]->Downstream << " down"
		    << "\n";
	    out << std::endl;
	}
	
	void
	ResultsPrinter::
	ClientTimes() const
	{
	    ProgramOptions* po = ProgramOptions::instance();
	    if (!po->Contains("-fctimes"))
		return;
	    std::string fileName = po->Value<string>("-fctimes");
	    std::ofstream out(fileName.c_str());
	    if (!out)
	    {
		std::cerr << "[RP] ERROR: Unable to open \"" << 
		    fileName << "\" for output." << std::endl;
		return;
	    }

	    const std::vector<Client*>& clients = 
		Scenario::instance()->Source.Clients();

	    int count = clients.size();
	    for(int index = 0; index < count; ++index)
	    {
		out << "\n" << index << "\t";
		if(clients[index]->Waits.size()) 
		    out << clients[index]->Waits[0];
	    }
	    out << std::endl;
	}
    }
}
