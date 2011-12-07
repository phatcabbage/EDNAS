#ifndef __TRACKER__CONTACTLIST_HXX__
#define __TRACKER__CONTACTLIST_HXX__

#include "SEDNAS.h++"

#include <list>
#include <set>

namespace Peer { class Peer; }

namespace Tracker
{
    /**
     * \brief Contains a random list of peers.
     *
     * This class is used by the Tracker and is sent via a TrackerResponse to
     * a Peer. Its purpose is to give the Peer a random subset of the Tracker's
     * contacts.
     *
     */
    class ContactList
    {
    public:
	typedef std::list<Peer::Peer*> OutputList;
	/** 
	 * \brief Determines whether the list is transferred in compact mode.
	 * BitTorrent defines two formats for the contact list:
	 * -# Dictionary mode
	 * -# Compact mode
	 *
	 * Compact mode is much smaller than the dictionary mode, but carries less
	 * information.
	 */
	const bool compact;

    private:
	typedef std::list<Peer::Peer*> InputList;
	/** Calculates the transfer size of the list. */
	static Datasize calcSize(uint numPeers, bool compactList);

	/**
	 * \brief Creates a randomized list customized to the contact Peer.
	 *
	 * This function takes the raw contact list that the Tracker sends
	 * and creates a randomized subset of it.
	 *
	 * \param inputList The raw list of Peers provided by the Tracker.
	 * \param contact The Peer for which this list is being created.
	 * \param listSize The desired size of the list.
	 * \post ContactList#_list will contain a random subset of \c inputList.
	 */
	static OutputList*
	createList(const InputList& inputList,
		   const Peer::Peer* contact,
		   uint listSize);

	/** The list of Peers that our contact Peer will receive. */
	OutputList* _list;

	ContactList(const ContactList&); // Prevent automatic copying

    public:
	/** The size (in bytes) of the ContactList. Used when transferring. */
	const Datasize dataSize;

	/**
	 * \brief Creates a ContactList.
	 *
	 * This constructor creates a ContactList of minimum size inputList.size() - 1.
	 * It assumes the caller desires to use the complete input list as a contact
	 * list, minus the Peer for which the list is being created; in other words,
	 * the list is just shuffled and the contact peer removed.
	 * 
	 * \param inputList The raw list of peers provided by the Tracker.
	 * \param contact The Peer for which the list is being made.
	 * \post ContactList#_list will contain a shuffled version of the inputList.
	 * \post ContactList#_list will not contain \c contact.
	 */
	ContactList(const InputList& inputList,
		    const Peer::Peer* contact,
		    bool a_compact);

	/**
	 * \brief Creates a ContactList of maximum size \c listSize.
	 *
	 * This constructor creates a ContactList with an upper bound on size.
	 * It will be a randomly-shuffled random subset of \c inputList.
	 *
	 * \param inputList The raw list of peers provided by the Tracker.
	 * \param contact The Peer for which the list is being made.
	 * \post ContactList#_list will contain a randomly shuffled subset of \c inputList.
	 * \post ContactList#_list will not contain \c contact.
	 * \post ContactList#_list will contain at most \c listSize contacts.
	 */
	ContactList(const InputList& inputList,
		    const Peer::Peer* contact,
		    uint listSize,
		    bool a_compact);

	~ContactList();

	/**
	 * \brief Returns the contents of the ContactList.
	 */
	const OutputList& list(void) const;
    };
}

#endif // __TRACKER__CONTACTLIST_HXX__
