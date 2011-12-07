#include "Simulation/Timer.h++"
#include "Protocols/Ethernet/Node.h++"

int main(int argc, char** argv)
{
    Ethernet::Node a, b;
    auto link = Links::PointToPoint;
    link.Connect(a,b);
    return 0;
}
