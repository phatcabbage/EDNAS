#ifndef PROTOCOLS__TFTP_HXX__
#define PROTOCOLS__TFTP_HXX__

#include "SEDNAS.h++"
#include "UDP.h++"

namespace Protocols
{
    using Protocols::UDP;

    struct TFTP
    {
	typedef IPv4 ContainerProtocol;
	const static UDP::SubType 
	TypeCode = UDP::TFTP;
	enum SubType
	{
	    ReadRequest,
	    WriteRequest,
	    Acknowledgment,
	    Data,
	    Error
	};

	struct Packet;
	typedef Packet PacketType;
	typedef std::shared_ptr<Packet> PacketPtr;

	const static UDP::Port ServerPort;
	
	template<SubType _S>
	struct Message;

	enum ErrorCode
	{
	    Undefined,
	    FileNotFound,
	    AccessViolation,
	    DiskFull,
	    IllegalOperation,
	    UnknownID
	};
	
	static const TimeType TIMEOUT = 3 * Time::SECOND;

	struct File
	{
	    std::string Name;
	    SizeType	Size;
	};
	
	typedef Message<ReadRequest>	ReadRequestMessage;
	typedef Message<WriteRequest>	WriteRequestMessage;
	typedef Message<Acknowledgment>	AcknowledgmentMessage;
	typedef Message<Data>		DataMessage;
	typedef Message<Error>		ErrorMessage;
	
	static const SizeType MAX_DATA = 512 * Size::BYTE;
    };

    struct TFTP::Packet
    {
	typedef unsigned short
	BlockID;

	TFTP::SubType	Type;
	SizeType	Length;

	SizeType
	Size() const
	{ return this->Length; }

	virtual ~Packet(){}
	
    protected:
	Packet(TFTP::SubType type,
	       SizeType	size)
	    : Type(type)
	    , Length(size)
	{}
    };
    
    template<>
    struct TFTP::Message<TFTP::ReadRequest>
	: public TFTP::Packet
    {
	const std::string FileName;
	
	Message(const std::string& s)
	    : Packet(TFTP::ReadRequest,
		     88 + (s.size() * Size::BYTE))
	    , FileName(s)
	{}
    };
    
    template<>
    struct TFTP::Message<TFTP::WriteRequest>
	: public TFTP::Packet
    {
	const std::string FileName;

	Message(const std::string& s)
	    : Packet(TFTP::WriteRequest, 
		     88 + (s.size() * Size::BYTE))
	    , FileName(s)
	{}
    };

    template<>
    struct TFTP::Message<TFTP::Data>
	: public TFTP::Packet
    {
	static const SizeType FULL_SIZE = MAX_DATA + (32 * Size::BIT);
	const BlockID Block;

	Message(BlockID block, 
		SizeType size = MAX_DATA)
	    : Packet(TFTP::Data, size + 32)
	    , Block(block)
	{}
    };
    
    template<>
    struct TFTP::Message<TFTP::Acknowledgment>
	: public TFTP::Packet
    {
	const BlockID Block;
	
	Message(BlockID block) 
	    : Packet(TFTP::Acknowledgment, 32)
	    , Block(block)
	{}
    };

    template<>
    struct TFTP::Message<TFTP::Error>
	: public TFTP::Packet
    {
	const TFTP::ErrorCode Code;
	const std::string ErrMsg;
	Message(TFTP::ErrorCode error = TFTP::Undefined, 
		const std::string& msg = std::string())
	    : Packet(TFTP::Error, 
		     (5 + msg.size()) * Size::BYTE)
	    , Code(error)
	    , ErrMsg(msg)
	{}
    };
}

#endif // PROTOCOLS__TFTP_HXX__
