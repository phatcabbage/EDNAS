#ifndef LINKS__LOSSYPOINTTOPOINT_HXX__
#define LINKS__LOSSYPOINTTOPOINT_HXX__

#include <cstdlib> // random()
#include "PointToPoint.h++"

namespace Network
{
  struct LossyPointToPoint : public PointToPoint
  {
	virtual void 
	Send(Side s, PacketListPtr& pkt)
	{
	  bool lose = ((double)rand() / RAND_MAX) > Reliability_;
	  if (!lose)
		PointToPoint::Send(s,pkt);
	}
	
	double
	Reliability() const
	{ return this->Reliability_; }

	LossyPointToPoint(double reliability = 1.0,
					  TimeType delay = 50 * Time::MILLISECOND,
					  SizeType leftUp = 64 * Bandwidth::KILOBIT,
					  SizeType rightUp = 64 * Bandwidth::KILOBIT)
	  : PointToPoint(delay, leftUp, rightUp)
	  , Reliability_(reliability)
	{}

  protected:
	double
	Reliability_;
  };
}
    
#endif // LINKS__LOSSYPOINTTOPOINT_HXX__
