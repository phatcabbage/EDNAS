#ifndef __PEERGROUP_HXX__
#define __PEERGROUP_HXX__

#include "SEDNAS.h++"
#include "Signaling.h++"
#include "Simulator.h++"
#include "Timing.h++"

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace Peer
{
    class Peer;

    class PeerGroup : public Signaling::Listener<Simulator>
    {
    private:
	struct PeerStartTimerEntry;
    protected:
	typedef 
	std::vector<Peer *> PeerList;
	
	/// A counter that keeps track of how many PeerGroups have been created.
	static 
	PeerGroupID 
	g_uidc;
	
	/// A map that maps PeerGroupID's to pointers to PeerGroups.
	static
	std::map<PeerGroupID, PeerGroup*> 
	g_peerGroupsToIds;
	
	/// The peers in this PeerGroup.
	/** This vector holds the peers that belong to this PeerGroup. */
	PeerList 
	_members;

	std::string 
	_name;
	/// The number of Peers in the group.
	
	uint 
	_size;

	/// The maximum upload rate (in kbit/s) of any given Peer in the group.
	Bandwidth 
	_maxUp;

	/// The maximum download rate (in kbit/s) of any given Peer in the group.
	Bandwidth 
	_maxDown;
	
	uint 
	_numSeeders;
	
	/// This holds a map of TimerIDs to Peers to tell us when to start them. It is generated at start().
	std::map<TimerID, Peer*> 
	_peerStartMap;
	
	void 
	createPeers();

	void
	setupSeeders();

	void 
	setupPeerStartTimes();

	void 
	setSeeder(Peer* p);
    
    public:
	/// The unique ID of this PeerGroup.
	const 
	PeerGroupID 
	id;
	
	/// Constructor.
	PeerGroup();

	PeerGroup(const AutoStr& a_name,
		        uint a_size,
		  const Bandwidth& a_maxUp,
		  const Bandwidth& a_maxDown,
		        uint a_numSeeders);
	    
	virtual 
	~PeerGroup();

	void 
	signal(const Signaling::Signal<Simulator>& sig);

	void 
	init();
	
	void 
	start();

	void 
	startPeer(TimerID id);

	void 
	save(std::ostream& o) const;

	void 
	load(std::istream& i);

	AutoStr
	description() const;

	inline 
	PeerList* 
	members()		        { return &(this->_members); }

	inline 
	std::string 
	name() const 	                { return this->_name; }

	inline 
	uint 
	size() const 		        { return this->_size; }

	inline 
	Bandwidth 
	maxUp() const 	                { return this->_maxUp; }

	inline 
	Bandwidth 
	maxDown() const 	        { return this->_maxDown; }

	inline 
	uint 
	numSeeders() const 	        { return this->_numSeeders; }

	inline 
	void 
	setName(const AutoStr& a_name)	{ this->_name = a_name; }

	inline 
	void 
	setSize(uint a_size) 		{ this->_size = a_size; }

	inline 
	void 
	setMaxUp(Bandwidth a_maxUp)     { this->_maxUp = a_maxUp; }

	inline 
	void 
	setMaxDown(Bandwidth a_maxDown) { this->_maxDown = a_maxDown; }

	inline 
	void 
	setNumSeeders(uint a_numS)	{ this->_numSeeders = a_numS; }

	bool 
	operator==(const PeerGroup& other) const;
    };
}

#endif // __PEERGROUP_HXX__
