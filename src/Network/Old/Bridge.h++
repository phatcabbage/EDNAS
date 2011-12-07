#ifndef NETWORK__SWITCH_HXX__
#define NETWORK__SWITCH_HXX__

#include <map>
#include <set>

#include "Logging/Logged.h++"
#include "Core/Signaling/Event.h++"
#include "./NIC.h++"

namespace Network
{
    typedef unsigned int MACAddress;
    struct Frame;
    struct NIC;
    struct Switch : Logging::Logged<Switch>
    {
    private:
	struct NICHandler;

	enum Action
	{
	    FORWARDALL,
	    FORWARDONE,
	    DROP
	};

	typedef std::map<NIC*, NICHandler>
	NICHandlerMapType;

	typedef std::map<MACAddress, Action>
	RoutingPolicyMapType;

	typedef std::map<MACAddress, NIC*>
	RoutingTableMapType;

	static unsigned int G_ID;

	NICHandlerMapType
	NICHandlers;

	RoutingPolicyMapType
	RoutingPolicies;

	RoutingTableMapType
	RoutingTable;

	void
	ForwardAll(Frame& f, NIC& sourceNic);
	
	void
	ForwardOne(Frame& f, NIC& destNic);
	
	void
	Drop(Frame& f);

	void
	Received(Frame& f, NIC& nic);

	bool
	IsLocalhost(MACAddress m) const;

	bool
	IsLocalhost(NIC& n) const;
    public:
	const unsigned int ID;
	
	Switch();
	~Switch(){}

	Switch&
	operator+=(NIC& nic);
	
	Switch&
	operator-=(NIC& nic);
     }; 

    struct Switch::NICHandler : public EventHandler<Frame&>
    {
	Switch& Switch_;
	mutable NIC* NIC_;

        NICHandler(Switch& s, NIC& nic)
	    : Switch_(s)
	    , NIC_(&nic)
	{}

	NICHandler(const NICHandler& n)
	    : Switch_(n.Switch_)
	    , NIC_(n.NIC_)
	{ n.NIC_ = NULL; }

	~NICHandler();
	void
	operator()(Frame& f)
	{ this->Switch_.Received(f, *(this->NIC_)); }
	
	void
	Send(Frame& f)
	{ this->NIC_->Send(&f); }
    };
}

#endif // NETWORK__SWITCH_HXX__
