/**
 * @file Architectures/ClientServer/Server.c++
 *
 * @author Sean Salmon
 */

#include "Architectures/ClientServer/Server.h++"
#include "Network/Network.h++"
#include "Network/Message.h++"
#include "Network/Node.h++"

/** Allocate space for static ServerID's (a typedef of ID<Server>) */
template<> uint ID<Architectures::ClientServer::Server,uint>::g_created = 0;
template<> uint ID<Architectures::ClientServer::Server,uint>::g_current = 0;

namespace Architectures
{
    namespace ClientServer
    {
	Server::
	Server(Net::Network& net,
	       const Files::File& file,
	       Delegate* d,
	       const BandwidthType& upstream,
	       const BandwidthType& downstream
	    )
	    : Net::Node(net, upstream, downstream)//, NULL),
	    , _delegate           (d)
	    , _file               (file)
	    , id                  ()
	{}
	
	Notification
	Server::
	Sent(TransferMessage* m)
	{
	    if (_delegate)
		_delegate->onSent(m);
	}

	Notification
	Server::
	Received(RequestMessage* m)
	{ 
	    if(_delegate) 
		_delegate->onReceived(m);
	}


	Action
	Server::
	sendFile(Client *c)
	{
	    FileTransfer *ft = new FileTransfer(&_file);
	    TransferMessage *m = new TransferMessage(this, c, ft);
	    this->Enqueue(m);
	}
    } // ClientServer
} // Architectures

std::ostream&
operator<<(std::ostream& o, const Architectures::ClientServer::Server& c)
{
    return o << "[ClientServer::Server " << c.id << "][" << 
	c.Upstream << " (up) | " << c.Downstream << " (down)]"; 
}
