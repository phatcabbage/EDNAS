#include "EthernetNode.h++"

namespace Logging
{
    template<>
    OStreamLogger
    Logged<Nodes::EthernetBase>::
    Logger("EthernetBase");
}

namespace Nodes
{
}
