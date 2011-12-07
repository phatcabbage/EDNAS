#ifndef NETWORK__LINK_HXX__
#define NETWORK__LINK_HXX__

#include "Core/Numerical/DataTypes.h++"

namespace Foundation
{
    namespace Net
    {
	struct ILink;
	struct Packet;

	struct INic
	{
	public:
	    bool 
	    Send(Packet* pkt) = 0;

	    bool
	    Receive(Packet* pkt) = 0;
	};

	template<typename _OWNER, typename _MEDIUM>
	struct TNic : public INic
	{
	    typedef _OWNER OwnerType;
	    typedef _MEDIUM MediumType;

	    bool 
	    Send(Packet* pkt)
	    {
		this->Medium.Send(this, pkt);
	    }

	    bool
	    Receive(Packet* pkt)
	    {
		this->Owner.Receive(this, pkt);
	    }

	protected:		
	    MediumType* Medium_;
	    
	    TNic(const MediumType& medium, 
		  const BandwidthType& upstream,
		  const BandwidthType& downstream)
		: INic(upstream, downstream)
		, Medium_(medium)
	    {}
	    virtual ~TNic() {}
	};
    }
}


#endif // NETWORK__LINK_HXX__
