#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include "Core/Environment/ProgramOptions.h++"
#include "Foundation/Simulator/Simulator.h++"
#include "./ResultsPrinter.h++"
#include "./Scenario.h++"

namespace Scenarios
{
    namespace PingsAPlenty
    {
	Scenario::
	Scenario()
	    : Singleton<Scenario>()
	    , Signaling::Listener<Foundation::Simulator>()
	    , Sim(Foundation::Simulator::instance())
	    , Source()
	    , Factory()
	{
	    Sim->connect(this);
	    std::cout << "[SCEN] Pings-A-Plenty Scenario created." << std::endl;
	}

	void
	Scenario::
	PrintStats()
	{
	    ResultsPrinter printer;
	    Core::Environment::ProgramOptions* Options = Core::Environment::ProgramOptions::instance();

	    if (Options->Contains("-fstart"))
	    {
		std::string filename = Options->Value<string>("-fstart");
		std::ofstream out(filename.c_str());
		printer.StartTimes(out);
	    }

	    if (Options->Contains("-fend"))
	    {
		std::string filename = Options->Value<string>("-fend");
		std::ofstream out(filename.c_str());
		printer.EndTimes(out);
	    }
	    if (Options->Contains("-ftime"))
	    {
		std::string filename = Options->Value<string>("-ftime");
		std::ofstream out(filename.c_str());
		printer.PingTimes(out);
	    }
	    if (Options->Contains("-ffull"))
	    {
		std::string filename = Options->Value<string>("-ffull");
		std::ofstream out(filename.c_str());
		printer.FullStats(out);
	    }
	}

	void
	Scenario::Init()
	{
	    this->Source.Init();
	    std::cout << "[SCEN] Pings-A-Plenty Scenario initialized." << std::endl;
	}

	void
	Scenario::Start()
	{
//	    this->Source.Start();
	    std::cout << "[SCEN] Pings-A-Plenty Scenario started." << std::endl;
	}

	void
	Scenario::Stop()
	{
//	    this->Source.Stop();
	    this->PrintStats();
	}

	void
	Scenario::Deinit()
	{
	    Singleton<Scenario>::release();
	}


	void
	Scenario::signal(const Signaling::Signal<Foundation::Simulator>& sig)
	{
	    if (sig == Foundation::Simulator::INIT)
		this->Init();
	    else if (sig == Foundation::Simulator::START)
		this->Start();
	    else if (sig == Foundation::Simulator::STOP)
		this->Stop();
	    else if (sig == Foundation::Simulator::DEINIT)
		this->Deinit();
	}

	Scenario::
	~Scenario() {}
    }
}
