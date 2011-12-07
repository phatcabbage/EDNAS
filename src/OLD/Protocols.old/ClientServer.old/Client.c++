/**
 * @file Architectures/ClientServer/Client.c++
 *
 * @author Sean Salmon
 */

#include "Architectures/ClientServer/Client.h++"
#include "Network/Message.h++"

/* Allocate space for the static ClientID (a typedef of ID<Client>) variables. */
template<> uint ID<Architectures::ClientServer::Client,uint>::g_created = 0;
template<> uint ID<Architectures::ClientServer::Client,uint>::g_current = 0;

namespace Architectures
{
    namespace ClientServer
    {
	Command
	Client::
	start()
	{
	    if (_delegate) 
		_delegate->onStart(this);
	    else
		requestFile();
	}

	Notification
	Client::
	Sent(RequestMessage* m)
	{
	    if (_delegate)
		_delegate->onSent(m);
	}
	

	Notification
	Client::
	Received(TransferMessage* msg)
	{
	    if (_delegate)
		_delegate->onReceived(msg);
	}

	Action
	Client::
	requestFile()
	{
//	    FileRequest*    r = new FileRequest();
	    RequestMessage* m = new RequestMessage(this, _server);
	    this->Enqueue(m);
	}
    } // ClientServer
} // Architectures

std::ostream&
operator<<(std::ostream& o, const Architectures::ClientServer::Client& c)
{
    return o << "[ClientServer::Client " << c.id << "][" <<
	c.Upstream << " (up) | " << c.Downstream << " (down)]"; 
}

