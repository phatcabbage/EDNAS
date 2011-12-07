#include <iostream>

#include "Core.h++"
#include "Protocols/HTTP.h++"
#include "SEDNAS.h++"
#include "Foundation/Foundation.h++"
#include "./Scenario.h++"
#include "./ClientController.h++"

namespace PH = Protocols::HTTP;

using namespace Scenarios::Webserver;

struct Stopper
{
    EventAdapter<Stopper, TimeType> ClockAdapter;
    const TimeType StopTime;
    void UpdateTime(const TimeType& t)
    {
	if (t >= StopTime) 
	    Foundation::Simulator::instance()->Stop();
    }
    Stopper(const TimeType& stoptime) 
	: ClockAdapter(this, &Stopper::UpdateTime)
	, StopTime(stoptime) 
    {
	Foundation::Simulator::instance()->Clock()->OnTick += this->ClockAdapter;
    }
};

int main(int argc, char** argv)
{
    double stopTime = 20;
    Core::Environment::ProgramOptions* po = Core::Environment::ProgramOptions::instance();
    po->Set(argc, argv);
    Foundation::Simulator* sim = Foundation::Simulator::instance();
    Scenario* scen = Scenario::instance();
    
    if (po->Contains("-stop"))
	stopTime = po->Value<double>("-stop");
//    Stopper s(TimeType(20));
    ClientController controller;

    Foundation::Simulator::instance()->Init();
    Foundation::Simulator::instance()->Start();

    return 0;
}  
