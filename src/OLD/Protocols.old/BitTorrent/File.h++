#ifndef PROTOCOLS__BITTORRENT__FILE_HXX_
#define PROTOCOLS__BITTORRENT__FILE_HXX_

#include <string>

#include "Core.h++"

namespace Protocols
{
    namespace BitTorrent
    {
	using std::string;

	class File
	{
	private:
	    /** A name or description for the file. */
	    string
	    Name_;

	    /** The file's size in bytes. */
	    DataType
	    Size_;

	    DataType
	    ChunkSize_;

	public:
	    /** 
	     * Creates a file with the given characteristics.
	     * @param a_name A name or description of the file.
	     * @param a_size The size (in bytes) of the file.
	     */
	    File(const string&   name,
		 const DataType& size,
		 const DataType& chunkSize)
		: Name_(name)
		, Size_(size)
		, ChunkSize_(chunkSize)
	    {}

	    /** @return The name of the file. */
	    inline 
	    const string&
	    GetName() const
	    { return this->Name_; }
    
	    /** Sets the file's name.
	     *  @post The file's name will be set to the new name.
	     */
	    inline
	    void
	    SetName(const string& name)
	    { this->Name_ = name; }

	    /** @return The size of the file. */
	    inline
	    const DataType&
	    GetSize() const
	    { return this->Size_; }

	    /** Sets the size of the file.
	     *  @post The file's size will be set to the new size.
	     */
	    inline
	    void
	    SetSize(const DataType& size)
	    { this->Size_ = size; }

	    inline
	    const DataType&
	    GetChunkSize() const
	    { return this->ChunkSize_;}

	    inline
	    void
	    SetChunkSize(const DataType& size)
	    { this->ChunkSize_ = size; }

	    inline
	    int
	    NumChunks() const
	    { return 1 + (int)(this->Size_ / this->ChunkSize_);}
	};
    } // BitTorrent
} // Protocols

#endif // PROTOCOLS__BITTORRENT__FILE_HXX_

