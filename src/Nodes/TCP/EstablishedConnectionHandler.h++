#ifndef NODES__TCP__ESTABLISHEDCONNECTIONHANDLER_HXX__
#define NODES__TCP__ESTABLISHEDCONNECTIONHANDLER_HXX__

namespace Nodes
{
    template<>
    struct ConnectionStateHandler<TCP::Established> : public TCPConnection::Handler
    {
    };
}

#endif // NODES__TCP__ESTABLISHEDCONNECTIONHANDLER_HXX__
