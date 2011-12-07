#include "Tracker/ContactList.h++"
#include "Peer/Peer.h++"

#include <algorithm>
#include <cassert>
#include <vector>

namespace Tracker
{
    ContactList::
    ContactList(const InputList& inputList,
		const Peer::Peer* contact,
		bool a_compact)
	: compact(a_compact),
	  _list(createList(inputList, contact, inputList.size())),
	  dataSize(calcSize(_list->size(), a_compact))
    {
	/* Left blank. */
    }
		
    ContactList::
    ContactList(const InputList& inputList,
		const Peer::Peer* contact,
		uint listSize,
		bool a_compact)
	: compact(a_compact),
	  _list(createList(inputList, contact, listSize)),
	  dataSize(calcSize(_list->size(), a_compact))
    {
	/* Left blank. */
    }

    Datasize
    ContactList::
    calcSize(uint numPeers, bool compactList)
    {
	return numPeers * (compactList ? 6 /* bytes for a compact list */
			   : (48)  /* dictionary (approx) */);
    }

    ContactList::
    ~ContactList()
    {
	if (_list) delete _list;
    }

    ContactList::OutputList*
    ContactList::
    createList(const InputList& inputList,
	       const Peer::Peer* contact,
	       uint listSize)
    {
	// To remove contact
	std::list<Peer::Peer*> scratchList(inputList.begin(), inputList.end());

	// To randomly sample
	std::vector<Peer::Peer*> scratch(scratchList.begin(), 
				   std::remove(scratchList.begin(), scratchList.end(), contact));

	std::random_shuffle(scratch.begin(), scratch.end());

	ulong size = (ulong) std::min((ulong)listSize, (ulong)scratch.size());

	std::vector<Peer::Peer*>::iterator first, last;
	first = scratch.begin();
	last = first + size;

	OutputList* result = new OutputList(first, last);
	return result;
    }

    const ContactList::OutputList&
    ContactList::
    list(void) const
    {
	return *_list;
    }
}
