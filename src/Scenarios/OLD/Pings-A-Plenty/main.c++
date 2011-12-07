#include <iostream>

#include "Foundation/Foundation.h++"
#include "./Scenario.h++"
#include "./NodeSource.h++"
#include "./NodeController.h++"
#include "Core/Environment/ProgramOptions.h++"

using namespace Scenarios::PingsAPlenty;

int main(int argc, char **argv)
{
    Core::Environment::ProgramOptions::instance()->Set(argc, argv);

    Scenario* __attribute__((unused)) scen = Scenario::instance();

    NodeController __attribute__((unused)) *Control = new NodeController();
    Foundation::Simulator::instance()->Init();
    Foundation::Simulator::instance()->Start();
    return 0;
}
