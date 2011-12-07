#ifndef NETWORK__NODE_HXX__
#define NETWORK__NODE_HXX__

#include <deque>
#include <list>
#include <map>
#include <queue>
#include <vector>

namespace Network
{
    template<template <typename _P, typename _P_Super, bool _IsLinkLayer> class _Protocol>
    struct Node;

    template<int _COUNT>
    struct SubTypeContainer
    {	typedef Type

    template<>
    struct Node<template <typename _P, typename _P_Super, true> class _Protocol>
    {
    protected:
	typedef Address<_P> AddressType;
	typedef Packet<_P> PacketType;
	typedef std::queue<PacketType> PacketQueueType;

	AddressType
	Address_;
	
	PacketQueueType
	PacketQueue_;

	void
	Send();

	void
	Sent(PacketType& p);
	
	// void
	// Receive(PacketType& p);
	
	void
	Received(PacketType& p);

    public:
	bool
	Enqueue(PacketType& p);
	
	Node();
	~Node();
    };

    template<>
    struct Node<template <typename _P, typename _P_Super, false> class _Protocol>
    {
    protected:
	typedef Address<_P> AddressType;
	typedef Packet<_P> PacketType;
	typedef std::queue<PacketType> PacketQueueType;

	AddressType
	Address_;
	
	void
	Received(PacketType& p);

    public:
	bool
	Enqueue(PacketType& p);
	
	Node();
	~Node();
    };

    template<>
    bool
    struct Node<template <typename _P, typename _P_Super, false> class _Protocol>::
    Enqueue
}

#endif // NETWORK__NODE_HXX__
