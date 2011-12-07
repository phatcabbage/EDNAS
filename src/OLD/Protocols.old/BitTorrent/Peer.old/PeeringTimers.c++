#include "Peering.h++"
#include "SLAAP.h++"

namespace Peer
{
    PeerController::PeerTimerEntry::PeerTimerEntry(Peer* a_p, PeerController::PeerTimerEntry::Function a_f) : p(a_p), f(a_f)
    {
	/* Left blank. */
    }

    void
    PeerController::PeerTimerEntry::execute(TimerID t)
    {
	(p->*f)();
    }

    PeerController::UpdateTimerEntry::UpdateTimerEntry(PeerController* a_pc, PeerController::UpdateTimerEntry::Function a_f) : pc(a_pc), f(a_f)
    {
	/* Left blank. */
    }

    void
    PeerController::UpdateTimerEntry::execute(TimerID)
    {
	(pc->*f)();
    }

    
    PeerGroup::PeerStartTimerEntry::PeerStartTimerEntry(PeerGroup *pg, Peer *p) : group(pg), peer(p)
    {
	/* Left blank. */
    }

    void
    PeerGroup::PeerStartTimerEntry::execute(TimerID id)
    {
	this->group->startPeer(id);
    }
}
