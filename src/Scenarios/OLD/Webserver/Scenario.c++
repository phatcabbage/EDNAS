#include <iostream>
#include "Core/Environment/ProgramOptions.h++"
#include "./Client.h++"
#include "./ResultsPrinter.h++"
#include "./Scenario.h++"


namespace Scenarios
{
    namespace Webserver
    {
	Scenario::
	Scenario()
	    : Singleton<Scenario>()
	    , Signaling::Listener<Foundation::Simulator>()
	    , File("File", DataType(DEF_FILE_SIZE))
	    , WebServer()
	    , Source()
	    , Factory()
	    , Controller()
	{
	    Core::Environment::ProgramOptions* Options =
		Core::Environment::ProgramOptions::instance();

	    if (Options->Contains("-filesize"))
		this->File.SetSize(DataType(Options->Value<double>("-filesize")));
	    Foundation::Simulator::instance()->connect(this);
	    std::cout << "[SCEN] Webserver Scenario created." << std::endl;
	}
	void
	Scenario::Init()
	{
	    this->WebServer.Resource = &(this->File);
	    this->Source.Init();
	    this->Controller.Init();
	    std::cout << "[SCEN] Webserver Scenario initialized." << std::endl;
	}

	void
	Scenario::Start()
	{
	    std::cout << "[SCEN] Webserver Scenario started." << std::endl;
	}

	void
	Scenario::Stop()
	{
	    std::cout << "[SCEN] Webserver Scenario stopped." << std::endl;
	    ResultsPrinter printer;
	    printer();
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
