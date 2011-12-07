#include <iostream>
#include "Simulation/Timer.h++"
#include "Nodes/Bridge.h++"
#include "Nodes/Pinger.h++"

int main(int argc, char** argv)
{
    Logging::Logged<Simulation::Timer>::
	Verbosity(Logging::Warning);
    Logging::Logged<Nodes::Bridge>::
	Verbosity(Logging::None);

    Nodes::EthernetNode ethnodeA, ethnodeB;
    Nodes::IPNode ipnodeA(ethnodeA), ipnodeB(ethnodeB);
    Nodes::Pinger pa(ipnodeA), pb(ipnodeB);
    Nodes::Bridge b;
    b += (pa.Node().Interface());
    b += (pb.Node().Interface());
    pa.SendPing(pb.Node().Address());
    Simulation::Timer::Main.Run();
    return 0;
}
