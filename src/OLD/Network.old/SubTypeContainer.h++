#ifndef NETWORK__SUBTYPECONTAINER_HXX__
#define NETWORK__SUBTYPECONTAINER_HXX__

#include <map>
#include <vector>

namespace Network
{
    template<typename _P, int _N>
    struct SubTypeContainer
    { typedef SubTypeContainer<_P, _N-1> Type };
    
    template<typename _P, 8>
    struct SubTypeContainer
    {
	typedef 
	std::map<
	    Address<_P>::Type,
	    Node<Protocol<_P>*> > 
	Type;
    };

    template<typename _P, 2>
    struct SubTypeContainer
    {
	typedef
	std::vector<Node<Protocol<_P>*> >
	Type;
    };

    template<typename _P, 0>
    struct SubTypeContainer
    {
	typedef
	Node<Protocol<_P>*>
	Type;
    };
}

#endif // NETWORK__SUBTYPECONTAINER_HXX__
