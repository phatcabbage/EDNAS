#include "Peering.h++"
#include "Simulator.h++"
#include "Clock.h++"
#include "Timer.h++"
#include "File.h++"

#include <cstdio>
#include <fstream>

#include <boost/random.hpp>

namespace Peer
{
    const Time MEAN_PEER_START_TIME = 300; // Corresponds to an average peer start time of 5 minutes  (if i remember what the unit of Time is ... )
    const Time PEER_START_TIME_STD_DEV = 60;

    PeerGroupID PeerGroup::g_uidc = 0;
    std::map<PeerGroupID, PeerGroup*> PeerGroup::g_peerGroupsToIds;

    PeerGroup::PeerGroup(void)
	: Signaling::Listener<Simulator>(),
	  _name(""),
	  _size(0),
	  _maxUp(0),
	  _maxDown(0),
	  _numSeeders(0),
	  id(g_uidc++)
    {
	PeerGroup::g_peerGroupsToIds[id] = this;
	Simulator::instance()->connect(this);
    }

    PeerGroup::PeerGroup(const std::string& a_name,
			 unsigned int a_size,
			 Bandwidth a_maxUp,
			 Bandwidth a_maxDown,
			 unsigned int a_numSeeders)
	: _name(a_name),
	  _size(a_size),
	  _maxUp(a_maxUp),
	  _maxDown(a_maxDown),
	  _numSeeders(a_numSeeders),
	  id(g_uidc++)
    {
	PeerGroup::g_peerGroupsToIds[id] = this;
	Simulator::instance()->connect(this);
    }

    PeerGroup::~PeerGroup()
    {
	PeerGroup::g_peerGroupsToIds.erase(id);

	foreach(Peer *p, _members)
	    delete p;
    }

    void
    PeerGroup::signal(const Signaling::Signal<Simulator>& sig)
    {
	if(sig == Simulator::INIT)
	{
	    this->init();
	    return;
	}
	if(sig == Simulator::START)
	{
	    this->start();
	    return;
	}
	if(sig == Simulator::STOP)
	{
	}
    }

    void
    PeerGroup::init(void)
    {
	this->createPeers();
	this->setupSeeders();
    }

    void
    PeerGroup::start(void)
    {
	if (_size > _numSeeders)
	    this->setupPeerStartTimes();
    }

    void
    PeerGroup::createPeers(void)
    {
	Peer *p;
	for (unsigned int index = 0; index < _size; index++)
	{
	    p = new Peer(this);
	    _members.push_back(p);
	}
    }

    void 
    PeerGroup::setupSeeders(void)
    {
	if (_numSeeders == _size)
	{
	    foreach (Peer *p, _members)
	    {
		this->setSeeder(p);
	    }
	    return;
	}

	PeerList::iterator seedIt = _members.begin();

	for (unsigned int seedIndex = 0; seedIndex < _numSeeders; seedIndex++, seedIt++)
	    this->setSeeder((*seedIt));
    }    
 
    void 
    PeerGroup::setupPeerStartTimes(void)
    {
	Clock *clock = Clock::instance();
	Timer *timer = Timer::instance();

	Time time = clock->time();   

	boost::mt19937 engine;
	boost::normal_distribution<double> distribution(MEAN_PEER_START_TIME, PEER_START_TIME_STD_DEV);
	boost::variate_generator<boost::mt19937, boost::normal_distribution<double> > numberSource(engine, distribution);
	PeerList::iterator peerIt = _members.begin();
	unsigned int peerIndex = 0;

	// Seeders get started now, at the beginning
	for(; peerIndex < _numSeeders; peerIndex++, peerIt++)
	    (*peerIt)->start();
    
	// The rest of the peers join at randomized times
	for(; peerIndex < _size; peerIndex++, peerIt++)
	{
	    Peer *p = (*peerIt);
	    Time startTime = (Time) numberSource() + time;
	    TimerID id = timer->create(startTime, new PeerStartTimerEntry(this,p));
	    _peerStartMap.insert(std::pair<TimerID,Peer*>(id, p));
	}
    }

    void
    PeerGroup::setSeeder(Peer *p)
    {
	ChunkID HighestChunkId = Simulator::instance()->file()->chunkCount();
	for(ChunkID cId = 0; cId < HighestChunkId; cId++)
	    p->bestowChunk(cId);
    }

    void
    PeerGroup::startPeer(TimerID tid)
    {
	Peer *p = _peerStartMap[tid];
	if (!p) return;
	p->start();
    }

    void
    PeerGroup::save(std::ostream& o) const
    {
	o << _name 		<< std::endl;
	o << _size 		<< std::endl;
	o << _maxUp 		<< std::endl;
	o << _maxDown 		<< std::endl;
	o << _numSeeders 	<< std::endl;
    }

    void
    PeerGroup::load(std::istream& i)
    {
	i >> _name;
	i >> _size;
	i >> _maxUp;
	i >> _maxDown;
	i >> _numSeeders;
    }

    std::string
    PeerGroup::description(void) const
    {
	char *desc;
	asprintf(&desc, "[(PeerGroup) Name: %s | Size: %d | Max Up: %f (kB/s) | Max Down: %f (kB/s) | Seeders: %d ]", _name.c_str(), _size, _maxUp, _maxDown, _numSeeders);
	std::string result(desc);
	free(desc);
	return result;
    }

    bool
    PeerGroup::operator==(const PeerGroup& other)
    {
	bool result = true;
	result &= (this->_name == other._name);
	result &= (this->_size == other._size);
	result &= (this->_maxUp == other._maxUp);
	result &= (this->_maxDown == other._maxDown);
	result &= (this->_numSeeders == other._numSeeders);
	return result;
    }
}
