#ifndef __ARCHITECTURES__CLIENT_SERVER__SERVER_HXX__
#define __ARCHITECTURES__CLIENT_SERVER__SERVER_HXX__

/**
 * @file Architectures/ClientServer/Server.h++
 *
 * Creates the interface for ClientServer's Server class.
 *
 * @author Sean Salmon
 */

#include "SEDNAS.h++"
#include "Architectures/ClientServer/ClientServer.h++"
#include "Network/Node.h++"

#include <cassert> // for assertions

/* Forward declaration of Files::File */
namespace Files {class File;}

namespace Net
{
    class Network;
}

namespace Architectures
{
    namespace ClientServer
    {
	/**
	 * @brief Serves {@link Files::File Files} to {@link Client Clients}.
	 *
	 * A Server waits for {@link RequestMessage RequestMessages} from Clients.
	 * Upon receipt, it sends its file to them in a {@link TransferMessage}.
	 */
	class Server : public Net::Node
	{
	public:
	    interface Delegate
	    {
	    	DelegateMethod
	    	onReceived(RequestMessage* msg) {}

		DelegateMethod
		onSent(TransferMessage* msg) {}
	    };

	    enum {DEF_UPSTREAM = 1048576,    /**< Default upstream bandwidth.   */
		  DEF_DOWNSTREAM = 1048576}; /**< Default downstream bandwidth. */


	    /** The Server's file. */
	    const
	    Files::File& 
	    _file;

	    /** The Server's id. */
	    const 
	    ServerID
	    id;

	    Delegate* 
	    _delegate;

	protected:
	public:
	    /**
	     * @brief Creates a Server.
	     * 
	     * @param file       The {@link Files::File File} the Server will be serving.
	     * @param upstream   The Server's upstream bandwidth.
	     * @param downstream The Server's downstream bandwidth.
	     *
	     * @pre \c file should not be NULL.
	     */
	    Server(Net::Network& net,
		   const Files::File& file,
		   Delegate* d = NULL,
		   const BandwidthType& upstream = BandwidthType(DEF_UPSTREAM),
		   const BandwidthType& downstream = BandwidthType(DEF_DOWNSTREAM)
		);

	    virtual
	    ~Server() {}

	Commands:
	    /**
	     * Sends the file to a client. Done in response to receiving a
	     * {@link RequestMessage}.
	     * 
	     * @param c The Client to which to send the file.
	     * @pre \c c should be a valid Client.
	     */
	    Command
	    sendFile(Client* c);

	// Events:
	//     /** Called in response to receipt of a RequestMessage. */
	//     Event<const RequestMessage&>
	//     onReceive;

	Accessors:
	    

	Notifications:
	    Notification
	    Sent(TransferMessage* m);

	    /** Notification that the Server has received a RequestMessage. */
	    Notification
	    Received(RequestMessage* m);
	};
    } // ClientServer
} // Architectures

#endif // __ARCHITECTURES__CLIENT_SERVER__SERVER_HXX__
