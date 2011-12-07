#ifndef __PEER__PEERINGTIMERS_HXX__
#define __PEER__PEERINGTIMERS_HXX__

#include "./PeerController.h++"
#include "./PeerGroup.h++"
#include "../Timing.h++"

namespace Peer
{
    class Peer;

    using ::Timing::TimerEntry;

    struct PeerController::PeerTimerEntry : public TimerEntry
    {
	typedef void (Peer::*Function)();
	Peer* const p;
	Function f;
	
	PeerTimerEntry(Peer* a_p, Function a_f);
	virtual void execute(TimerID);
    };

    struct PeerController::UpdateTimerEntry : public TimerEntry
    {
	typedef void (PeerController::*Function)();
	PeerController* const pc;
	Function f;
	
	UpdateTimerEntry(PeerController* a_pc, Function a_f);
	virtual void execute(TimerID);
    };

    struct PeerGroup::PeerStartTimerEntry : public TimerEntry
    {
	PeerGroup *group;
	Peer *peer;

	PeerStartTimerEntry(PeerGroup* pg, Peer* p);
	virtual void execute(TimerID);
    };
}

#endif
