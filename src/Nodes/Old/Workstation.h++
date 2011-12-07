#ifndef NETWORK__WORKSTATION_HXX__
#define NETWORK__WORKSTATION_HXX__

//#include 

#include <iosfwd>
#include <map>
#include <utility>

#include "Core/Signaling/EventAdapter.h++"
#include "Logging/Logged.h++"

#include "./IPNode.h++"
#include "./Pinger.h++"

namespace Nodes
{
    typedef Pinger Workstation;
}

#endif // NETWORK__WORKSTATION_HXX__
