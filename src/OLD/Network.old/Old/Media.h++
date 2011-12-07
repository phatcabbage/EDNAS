#ifndef NETWORK__MEDIA_HXX__
#define NETWORK__MEDIA_HXX__

#include <utility> // std::pair
#include <deque> // queue
#include <list>

namespace Foundation
{
    namespace Net
    {
	struct INic;
	struct Packet;

	struct Medium
	{
	public:
	    const BandwidthType 
	    Bandwidth;

	    const TimeType 
	    Delay;
	    
	    bool Send(const INic* const iNic,
		      Packet* const pkt) = 0;
	    
	    Medium(const BandwidthType& bandwidth,
		   const TimeType& delay)
		: Bandwidth(bandwidth)
		, Delay(delay) {}

	    virtual ~Medium() {}
	};	    

	struct PointToPoint
	{
	public:
	    bool Send(const INic* const iNic,
		      Packet* const pkt);
	protected:
	    typedef 
	    std::queue<Packet*> 
	    PacketQueue;

	    const std::pair<INic*,INic*>
	    Ends_;
	    
	    std::pair<PacketQueue,PacketQueue>
	    Queues_;
	    
	    PointToPoint(INic* link0, INic* link1,
			 BandwidthType bandwidth,
			 TimeType delay)
		: Medium(bandwidth, delay)
		, Ends_(std::make_pair(link0, link1))
		, Queues_(std::make_pair<PacketQueue(), PacketQueue()) {}
	    virtual ~PointToPoint() {}
	};
	
	struct MultipleAccess : public Medium
	{
	public:
	    void Add(INic* const iNic);
	    void Remove(INic* const iNic);
	    void Send(const INic* const link,
		      Packet* pkt);
	protected:
	    std::list<INic*> Nics_;
	};
    }
}

#endif // NETWORK__MEDIA_HXX__
