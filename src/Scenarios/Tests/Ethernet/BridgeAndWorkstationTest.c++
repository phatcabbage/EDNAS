#include <cstdlib> // rand()
#include <ctime> // time()
#include <iostream>
#include <list>
#include <vector>
#include "Simulation/Timer.h++"
#include "Nodes/IPNode.h++"
#include "Nodes/Workstation.h++"
#include "Nodes/Bridge.h++"
#include "Protocols/IPv4.h++"

static const int NODES_PER_GROUP = 5;
static const int NUM_GROUPS = 3;
static const int NUM_PINGS = 128;

using Protocols::IPv4;

std::vector<Nodes::Workstation*>
G_Workstations;

struct Group
{
    static unsigned int GROUP_ID;
    const unsigned int ID;

    std::vector<Nodes::Workstation*> Workstations;

    Nodes::Bridge Bridge;

    Group(int numWorkstations)
	: ID(++GROUP_ID)
	, Workstations()
	, Bridge()
    {
	for(int i = 0; i < numWorkstations; ++i)
	{
// 	    std::cerr << "((ID << 16) + i) = ((" << ID << " << 16) + " << i
// 		      << ") = " << ((ID << 16) + i) << std::endl;
	    IPv4::AddressType ip = ((ID << 16) + i);
	    Nodes::EthernetNode* ethnode = new Nodes::EthernetNode;
	    Nodes::IPNode* node = new Nodes::IPNode(*ethnode, ip);
//	    node->Address(ip);
	    Nodes::Workstation* wk = 
		new Nodes::Workstation(*node);
//	    Nodes.push_back(node);
	    Workstations.push_back(wk);
	    G_Workstations.push_back(wk);
	}

	for(auto it = Workstations.begin(), end = Workstations.end();
	    it != end; ++it)
	    this->Bridge += ((*it)->Node().Interface());
    }
    ~Group()
    {
	for(auto it = this->Workstations.rbegin(), rend = this->Workstations.rend();
	    it != rend; ++it)
	{
	    Nodes:: Workstation* w = *it;
	    Nodes::IPNode* i = &(w->Node());
	    Nodes::EthernetNode* e = &(i->Interface());
	    delete w;
	    delete i;
	    delete e;
	}
    }
    Group&
    operator+=(Group& other)
    {
	this->Bridge += other.Bridge;
	return *this;
    }
};

unsigned int
Group::
GROUP_ID(0);

void CreateRandomSend()
{
    unsigned int Sender = rand() % G_Workstations.size();
    unsigned int Receiver = rand() % G_Workstations.size();
    if (Sender == Receiver) { CreateRandomSend(); return; }
    Nodes::Workstation* s = G_Workstations[Sender];
    Nodes::Workstation* r = G_Workstations[Receiver];
    s->SendPing(r->Node().Address());
}

int main(int arg, char **argv)
{
    Logging::Logged<Nodes::Bridge>::Verbosity(Logging::None);
    Logging::Logged<Nodes::Workstation>::Verbosity(Logging::None);

    Group a(5),b(3),c(7);
    b += c;
    a += b;

    for(int i = 0; i < NUM_PINGS; ++i)
	Simulation::Timer::Main.Schedule(.5 * i, CreateRandomSend);
    
    Simulation::Timer::Main.Run();
    std::cout << "Main finished." << std::endl;
    return 0;
}
