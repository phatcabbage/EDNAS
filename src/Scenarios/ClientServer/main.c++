#include <cstring>
#include <sstream>
#include "ClientServer.h++"
#include "Simulation/Timer.h++"
#include "Network/PointToPoint.h++"

std::string ProgName;

int G_NUM_CLIENTS = 3;
int G_NUM_SERVERS = 1;

SizeType G_CLIENT_BW = 128 * Bandwidth::KILOBIT;
SizeType G_SERVER_BW = 128 * Bandwidth::KILOBIT;

SizeType G_CLIENT_DELAY = 30 * Time::MILLISECOND;
SizeType G_SERVER_DELAY = 30 * Time::MILLISECOND;

SizeType G_LINK_MAXQUEUE = 1 * Size::MEGABYTE;

SizeType G_MSGSIZE = 1 * Size::MEGABYTE;

static const int PORT = 37;

Logging::FileLogger
CSLog("ClientServer");

void
ParseOpts(std::list<char*> args);

void
PrintHelp();

void
PrintOpts();

template<typename T>
T 
Convert(const char* cs)
{
    static std::istringstream iss;
    std::string s(cs);
    T val;
    iss.str(s);
    iss >> val;
    return val;
}

using namespace ClientServer;

int main(int argc, char **argv)
{
    CSLog << "STARTED\t" << TimeToString(Timer::Main.Time()) << Logging::endl;
    std::list<char*> args(argv, &argv[argc]);
    ParseOpts(args);
    PrintOpts();
    
    Subnet subnet(1, G_MSGSIZE, G_CLIENT_BW, G_SERVER_BW,
		  G_CLIENT_DELAY, G_SERVER_DELAY);
    
    for(int i = 0; i < G_NUM_CLIENTS; ++i)
	subnet.AddClient(G_CLIENT_BW, G_CLIENT_DELAY);

    for(int i = 0; i < G_NUM_SERVERS; ++i)
    {
	ServerPtr s = subnet.AddServer(G_SERVER_BW, G_SERVER_DELAY);
	s->Listen(PORT);
    }
    
    for(auto it = subnet.Clients.begin(), 
	    end = subnet.Clients.end(); it != end; ++it)
    {
	for(auto it2 = subnet.Servers.begin(),
		end2 = subnet.Servers.end(); it2 != end2;
	    ++it2)
	    (*it)->Connect(*(*it2), PORT, PORT);
    }
    
    Timer::Main.Run();
    CSLog << "FINISHED\t" << TimeToString(Timer::Main.Time()) << Logging::endl;
    return 0;
};

void
ParseOpts(std::list<char*> args)
{
    ProgName = args.front();
    args.pop_front();

    for(auto it = args.begin(), end = args.end();
	it != end; ++it)
    {
	if(strcmp("-numclients", *it) == 0)
	{
	    G_NUM_CLIENTS = Convert<int>(*(++it));
	    if(!G_NUM_CLIENTS) { PrintHelp(); exit(1); }
	    continue;
	}
	else if(strcmp("-numservers", *it) == 0)
	{
	    G_NUM_SERVERS = Convert<int>(*(++it));
	    if(!G_NUM_SERVERS) { PrintHelp(); exit(1); }
	    continue;

	} 
	else if(strcmp("-clientbw", *it) == 0)
	{
	    G_CLIENT_BW = StringToBandwidth(*(++it));
	    if(!G_CLIENT_BW) { PrintHelp(); exit(1); }
	    continue;
	}
	else if(strcmp("-clientdelay", *it) == 0)
	{
	    G_CLIENT_DELAY = StringToTime(*(++it));
	    if(!G_CLIENT_DELAY) { PrintHelp(); exit(1); }
	    continue;
	}
	else if(strcmp("-serverbw", *it) == 0)
	{
	    G_SERVER_BW = StringToBandwidth(*(++it));
	    if(!G_SERVER_BW) { PrintHelp(); exit(1); }
	    continue;
	}
	else if(strcmp("-serverdelay", *it) == 0)
	{
	    G_SERVER_DELAY = StringToTime(*(++it));
	    if(!G_SERVER_DELAY) { PrintHelp(); exit(1); }
	    continue;
	}
	else if(strcmp("-linkbuffer", *it) == 0)
	{
	    G_LINK_MAXQUEUE = StringToSize(*(++it));
	    if(!G_LINK_MAXQUEUE) { PrintHelp(); exit(1); }
	    Network::PointToPoint::MAX_BUFFER = G_LINK_MAXQUEUE;
	    continue;
	}
	else if(strcmp("-msgsize", *it) == 0)
	{
	    G_MSGSIZE = StringToSize(*(++it));
	    if(!G_MSGSIZE) { PrintHelp(); exit(1); }
	    continue;
	}
	else if(strcmp("-help", *it) == 0)
	{
	    PrintHelp(); 
	    exit(0);
	}
	else
	{
	    PrintHelp();
	    exit(1);
	}
    }
}

void
PrintHelp()
{
    std::cerr << "Usage: " << ProgName << "<OPTIONS>\n"
	      << "\tOptions:\n"
	      << "\t\t-numservers NUM    \t\tCreate NUM clients\n"
	      << "\t\t-numclients NUM    \t\tCreate NUM servers\n"
	      << "\t\t-serverbw BANDWIDTH\t\tSet bandwidth between router and server-side bridge to BANDWIDTH Kbit/s\n"
	      << "\t\t-clientbw BANDWIDTH\t\tSet bandwidth between router and client-side bridge to BANDWIDTH Kbit/s\n"
	      << "\t\t-serverdelay DELAY\t\tSet delay between router and server-side bridge to DELAY ms\n"
	      << "\t\t-clientdelay DELAY\t\tSet delay between router and client-side bridge to DELAY ms\n"
	      << "\t\t-linkbuffer SIZE  \t\tSet the maximum amount of data a link can buffer to SIZE\n"
	      << "\t\t-msgsize SIZE     \t\tSet size of each server message to SIZE\n"
	      << std::endl;
}

void
PrintOpts()
{
    CSLog << "CLIENTS\t" << G_NUM_CLIENTS << Logging::endl;
    CSLog << "CLIENT BANDWIDTH\t" << BandwidthToString(G_CLIENT_BW) << Logging::endl;
    CSLog << "CLIENT DELAY\t" << TimeToString(G_CLIENT_DELAY) << Logging::endl;
    CSLog << "SERVERS\t" << G_NUM_SERVERS << Logging::endl;
    CSLog << "SERVER BANDWIDTH\t" << BandwidthToString(G_SERVER_BW) << Logging::endl;
    CSLog << "SERVER DELAY\t" << TimeToString(G_SERVER_DELAY) << Logging::endl;
    CSLog << "LINK BUFFER\t" << SizeToString(G_LINK_MAXQUEUE) << Logging::endl;
    CSLog << "MESSAGE SIZE\t" << SizeToString(G_MSGSIZE) << Logging::endl;
}
