#ifndef __ARCHITECTURES__CLIENTSERVER__CLIENTSERVER_HXX__
#define __ARCHITECTURES__CLIENTSERVER__CLIENTSERVER_HXX__

/**
 * @file ClientServer.h++
 *
 * A header that includes all the headers for the
 * Client/Server architecture.
 *
 * @author Sean Salmon
 *
 */

#include <ostream>
#include "SEDNAS.h++"

#include "Network/Network.h++"

// Forward declaration of ID class template
template<typename Class, typename IntType> 
class ID;

// Forward declaration of Transfer::Message class template
namespace Net
{
    template<typename Sender,typename Receiver, typename Payload>
    class Message; 
}

namespace Architectures
{ 

    /**
     * @namespace Architectures::ClientServer
     *
     * @brief Models a simple Client/Server network architecture.
     *
     * In this architecture, there are two types of actors: {@link Server Servers} and {@link Client Clients}.
     * Each server serves only one {@link File}, and each client only knows of one server. They communicate by means of
     * two messages, {@link RequestMessage} and {@link TransferMessage}. Once a client has requested a file,
     * the server sends it back as one long contiguous message.
     */
    namespace ClientServer 
    { 
	/* Actors in this architecture. */
	class Client; 
	class Server;

	/* The payload of Messages passed between the actors. */
	class FileTransfer;
	class FileRequest;

	/* Messages */

	/** 
	 * A message sent by a {@link Client} to a {@link Server} to request
	 * a {@link Files::File File}.
	 */
	typedef Net::Message<Client,Server,FileRequest>  RequestMessage;

	/**
	 * A message sent by a {@link Server} to a {@link Client}. It contains
	 * a {@link Files::File File}.
	 */
	typedef Net::Message<Server,Client,FileTransfer> TransferMessage;


	/* Unique IDs for the actors. */

	/** Uniquely identifies a {@link Client}. */
	typedef ID<Client,uint> ClientID;
	/** Uniquely identifies a {@link Server}. */
	typedef ID<Server,uint> ServerID;
    } // ClientServer
} // Architectures

#include "./Client.h++"
#include "./Server.h++"
#include "./Messages.h++"

std::ostream&
operator<<(std::ostream& o, const Architectures::ClientServer::Client& c);

std::ostream&
operator<<(std::ostream& o, const Architectures::ClientServer::Server& c);


#endif // __ARCHITECTURES__CLIENTSERVER__CLIENTSERVER_HXX__
