#ifndef NETWORK__PACKET_HXX__
#define NETWORK__PACKET_HXX__

#include <deque>
#include "SEDNAS.h++"

#include "Protocols/ProtocolList.h++"

namespace Network
{
    struct IData
    {
	virtual SizeType
	Size() const = 0;
    };

    struct IPacket : public IData
    {
	int Type;
	int PayloadType;
	int Receiver;
	int Sender;
	SizeType Length;
	bool HasData;

	IPacket(int type = 0,
		int payloadType = 0,
		int receiver = 0,
		int sender = 0,
		SizeType length = 0,
		bool hasData = false)
	    : Type(type)
	    , PayloadType(payloadType)
	    , Receiver(receiver)
	    , Sender(sender)
	    , Length(length)
	    , HasData(hasData)
	{}

	virtual ~IPacket(){}
	
	virtual SizeType
	Size() const
	{ return Length; }
    };

    typedef std::shared_ptr<IPacket> PacketPtr;

    struct PacketList : public IData
    {
	typedef std::vector<PacketPtr> PacketHolder;

	PacketList() : Packets() {}
	PacketList(const PacketList& o) : Packets(o.Packets) {}

	PacketPtr&
	operator[](size_t n)
	{ 
	    if (n >= Packets.size())
		Packets.resize(n + 1);
	    return Packets[n]; 
	}

	PacketPtr
	operator[](size_t n) const
	{ return Packets[n]; }

	PacketPtr&
	at(size_t n)
	{ return Packets[n]; }

	// PacketHolder::iterator
	// begin()
	// { return Packets.begin(); }

	// PacketHolder::iterator
	// end()
	// { return Packets.end(); }

	// PacketPtr
	// front() const
	// { return Packets.front(); }

	// PacketPtr
	// back() const
	// { return Packets.back(); }
	
	// size_t
	// size() const
	// { return Packets.size(); }

	// void
	// push_front(PacketPtr& p)
	// { Packets.push_front(p); }

	// void
	// push_back(PacketPtr& p)
	// { Packets.push_back(p); }
	
	// void
	// pop_front()
	// { Packets.pop_front(); }

	// void
	// pop_back()
	// { Packets.pop_back(); }

	bool
	empty() const
	{ return Packets.empty(); }

	size_t
	size() const
	{ return Packets.size(); }

	virtual
	SizeType Size() const
	{
	    if (Packets.empty() || !Packets[0])
		return 0;
	    else
		return (*Packets[0]).Size();
	}
	
    protected:
	PacketHolder
	Packets;
    };

    typedef std::shared_ptr<PacketList> PacketListPtr;
}

typedef Network::PacketListPtr PLP;

#endif // NETWORK__PACKET_HXX__
