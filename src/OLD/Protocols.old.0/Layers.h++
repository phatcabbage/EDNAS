#ifndef PROTOCOLS__LAYERS_HXX__
#define PROTOCOLS__LAYERS_HXX__

namespace Protocols
{
    struct Layers
    {
	enum Layer
	{
	    Link,
	    Internet,
	    Transport,
	    Data
	};
    };
}

#endif // PROTOCOLS__LAYERS_HXX__
