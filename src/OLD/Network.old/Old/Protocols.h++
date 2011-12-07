#ifndef NEWNETWORK__PROTOCOLS_HXX__
#define NEWNETWORK__PROTOCOLS_HXX__

namespace Network
{
    struct Ethernet
    {
	typedef unsigned int AddressType;
	typedef Message<_Ethernet> MessageType;
    };

    template<>
    struct Properties<Ethernet::AddressType>
    {
	enum { size = 4 };
    };
}

#endif // NEWNETWORK__PROTOCOLS_HXX__
