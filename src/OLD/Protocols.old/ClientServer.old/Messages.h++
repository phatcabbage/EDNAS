#ifndef __ARCHITECTURES__CLIENTSERVER__MESSAGES_HXX__
#define __ARCHITECTURES__CLIENTSERVER__MESSAGES_HXX__

#include "SEDNAS.h++"
#include "Architectures/ClientServer/ClientServer.h++"
#include "Files/File.h++"

/**
 * @file Architecture/ClientServer/Messages.h++
 *
 * Defines the messages that Client and Server use to communicate.
 *
 * @author Sean Salmon
 */

namespace Architectures
{
    namespace ClientServer
    {
	/**
	 * @brief Represents a {@link Files::File File} being transferred.
	 *
	 * Sent by a {@link Server} in response to a {@link Client Client's} {@link FileRequest}.
	 * It is the entire file crammed into one message. This is wrapped in a Transfer::Message
	 * for transmission.
	 */
	class FileTransfer
	{
	public:
	    /**
	     * The size in bytes of the message (which is the same as the 
	     * size of the {@link Files::File File}.
	     */
	    const 
	    DataType
	    size;
	    
	    /**
	     * @brief Creates a FileTransfer, which transfers a file. Sent by a Server to a Client.
	     * @pre  \c f must be a valid {@link Files::File File}.
	     */
	    FileTransfer(const Files::File* f) : size(f->Size()) {}
	    
	    /** @return The size of the message. This returns the exact same value as #size. */
	    DataType
	    Size() { return size; }
	};

	/**
	 * @brief Represents a request for a {@link Files::File File}.
	 *
	 * Sent by a {@link Client} to {@link Server} request a {@link Files::File File}.
	 * This is wrapped in a Transfer::Message for transmission.
	 */
	class FileRequest
	{
	private:
	    enum {DATA_SIZE = 16 /**< This message is a constant 16 bytes. */};

	public:
	    /** The size in bytes of the message. */
	    const
	    DataType 
	    size;

	    /** @brief Creates a FileRequest, which asks a {@link Server} to send a {@link Files::File File}. */
	    FileRequest() : size(DataType(DATA_SIZE)) {}

	    /** @return The size of the message. */
	    DataType 
	    Size() { return size;}
	};
    } // ClientServer
} // Architectures

#endif // __ARCHITECTURES__CLIENTSERVER__MESSAGES_HXX__
