#include "Simulation/Timer.h++"
#include "Network/Workstation.h++"

using namespace Network;

int main(int argc, char** argv)
{
    Workstation wkA, wkB;
    NIC::Connect(&(wkA.GetNIC()), &(wkB.GetNIC()));
    wkA.SendPing(wkB.IP);
    Simulation::Timer::Main.Run();
    return 0;
}
