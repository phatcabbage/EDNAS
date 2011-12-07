#include <iostream>
#include "./Frame.h++"

namespace Network
{
    inline std::ostream&
    operator<<(std::ostream& o, const Frame& f)
    { 
	o << "|(" << f.Sender << "=>" << f.Receiver 
	  << ")" << f.Size << "b ";
	switch(f.Type)
	{ 
	case Frame::IPv4:
	    o << "IPv4";
	    break;
	case Frame::ARP:
	    o << "ARP";
	    break;
	default:
	    o << "(Unimportanté)";
	    break;
	}
	return o;
    }
}
