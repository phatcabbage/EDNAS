#ifndef PROTOCOLS__PING__MESSAGES_HXX__
#define PROTOCOLS__PING__MESSAGES_HXX__

#include <iostream>

#include "Foundation/Network/Message.h++"
#include "SEDNAS.h++"

namespace Protocols
{
    namespace Ping
    {
	struct Ping
	{
	    enum { _Size = 70};

	    Ping()
	    {
		std::cout << "[PM] Ping message created." << std::endl;
	    }

	    DataType
	    Size() const
	    {
		return DataType(_Size);
	    }
	};

	struct Pong
	{
	    enum { _Size = 70};

	    Pong()
	    {
		std::cout << "[PM] Pong message created." << std::endl;
	    }
	    
	    DataType
	    Size() const
	    {
		return DataType(_Size);
	    }
	};
	typedef Foundation::Net::Message<Ping> PingMessage;
	typedef Foundation::Net::Message<Pong> PongMessage;
    }
}

#endif // PROTOCOLS__PING__MESSAGES_HXX__
