#include "Simulation/Timer.h++"

#include "Nodes/Bridge.h++"
#include "Nodes/DHCPServer.h++"
#include "Nodes/DHCPClient.h++"

int main(int argc, char** argv)
{
    Logging::Logged<Simulation::Timer>::Verbosity(Logging::Warning);
    Logging::Logged<Nodes::DHCPServer>::Verbosity(Logging::None);
    Logging::Logged<Nodes::DHCPClient>::Verbosity(Logging::None);

    Nodes::Bridge bridge;

    Nodes::EthernetNode 
	eServe,
	eB,
	eC,
	eD;

    bridge += eServe;
    bridge += eB;
    bridge += eC;
    bridge += eD;

    Nodes::IPNode iServe(eServe), iB(eB), iC(eC), iD(eD);
    iServe.Address(1);

    Nodes::UDPNode uServe(iServe), uB(iB), uC(iC), uD(iD);

    Nodes::DHCPServer dServe(uServe, 2, 254);

    Nodes::DHCPClient dB(uB), dC(uC), dD(uD);
    
    dB.StartDiscovery();
    dC.StartDiscovery();
    dD.StartDiscovery();
    
    Simulation::Timer::Main.Run();

    std::cout << "Nodes B, C, D <=> IPs " 
	      << (iB.Address()) << " ,"
	      << (iC.Address()) << " ," 
	      << (iD.Address()) << std::endl;
    return 0;
}
