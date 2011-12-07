#ifndef HTTP__PROTOCOL_HXX__
#define HTTP__PROTOCOL_HXX__

#include "SEDNAS.h++"

namespace HTTP
{
    struct Message;
    typedef std::string URI;

    struct Protocol
    {
	enum ResponseCode
	{
	    Success,
	    Redirect,
	    ClientError,
	    ServerError
	};
    };
}

#endif // HTTP__PROTOCOL_HXX__
