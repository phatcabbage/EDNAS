#ifndef __ARCHITECTURE__CLIENT_SERVER__CLIENT_HXX__
#define __ARCHITECTURE__CLIENT_SERVER__CLIENT_HXX__

/**
 * @file Architectures/ClientServer/Client.h++
 *
 * Creates the interface for ClientServer's Client class.
 *
 * @author Sean Salmon
 *
 */

#include "SEDNAS.h++"
#include "Architectures/ClientServer/ClientServer.h++"
#include "Util/ID.h++" // for ClientID ( ID<Client> )
#include "Network/Node.h++"

/* Forward declarations */
namespace Files { class File;}

namespace Net
{
    class Network;
}

namespace Architectures
{
    namespace ClientServer
    {
	/**
	 * @brief Actor whose sole purpose is to request and receive
	 * {@link Files::File Files} from a {@link Server}.
	 */
	class Client
	    : public Net::Node
	{
	public:
	    interface Delegate
	    {
	    	DelegateMethod
		onStart  (Client* c) = 0;

	    	DelegateMethod
	    	onReceived(TransferMessage* msg){}

		DelegateMethod
		onSent(RequestMessage* msg){}
	    };		

	    enum {DEF_UPSTREAM = 131072,    /**< Default upstream bandwidth. */
		  DEF_DOWNSTREAM = 131072}; /**< Default downstream bandwidth */

	    Delegate*
	    _delegate;

	private:
	    /** The (@link Server} from which to download files. */
	    Server* 
	    _server;

	    
	Commands:
	    /*----------*
	     * Commands *
	     *----------*/

	    /** 
	     * Makes the client to request a client from its server 
	     * via a {@link RequestMessage}.
	     *
	     * @pre The #_server should be a valid {@link Server}.
	     */
	    Command
	    requestFile();

	public:

	    /*------------------*
	     * {Con,De}structor *
	     *------------------*/

	    /** A unique identifier for the Client. */
	    const
	    ClientID
	    id;

	    /** 
	     * @brief Creates a client.
	     * 
	     * @param s          The {@link Server} from which to request files.
	     * @param upstream   The client's upstream bandwidth.
	     * @param downstream The client's downstream bandwidth.
	     */
	    Client(Net::Network& net,
		   Server*              s          = NULL,
		   Delegate*            d          = NULL,
		   const BandwidthType& upstream   = BandwidthType(DEF_UPSTREAM),
		   const BandwidthType& downstream = BandwidthType(DEF_DOWNSTREAM)
		)
		: Net::Node(net,upstream, downstream)//, NULL),
		, _delegate(d)
		, _server(s)
		, id()
	    {}

	    virtual
	    ~Client() {}


	    /*-----------*
	     * Accessors *
	     *-----------*/

	    /** @return The client's server. */
	    Server*
	    server()
	    { return _server;}

	    /** Sets the client's server. */
	    void
	    setServer(Server* s)
	    { _server = s;}

	    /*----------*
	     * Commands *
	     *----------*/

	    /** Instructs the client to start; it then immediately requests a file from its server. */
	    Command
	    start();

	    /*---------------*
	     * Notifications *
	     *---------------*/
	    
	    /** Notifies the client that it has finished sending a request for a file. */
	    Notification
	    Sent(RequestMessage* m);

	    /** Notifies the client that it has finished receiving a requested file. */
	    Notification
	    Received(TransferMessage* m);
	};
    } // ClientServer
} // Architectures

#endif // __ARCHITECTURE__CLIENT_SERVER__CLIENT_HXX__
