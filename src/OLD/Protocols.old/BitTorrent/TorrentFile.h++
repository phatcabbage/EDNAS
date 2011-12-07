#ifndef __ARCHITECTURES__BITTORRENT__TORRENTFILE_HXX__
#define __ARCHITECTURES__BITTORRENT__TORRENTFILE_HXX__

#include "SEDNAS.h++"
#include "./BitTorrent.h++"
#include "Files/File.h++"

#include <vector>

namespace Architectures
{
    namespace BitTorrent
    {
	class TorrentFile : public Files::File
	{
	public:

	    enum
	    {
		DEF_SIZE = (1048576 * 1024), /* a jiggabyte */
		DEF_CHUNKSIZE = 262144, /* 256KB */
	    };
	
	    const
	    FileID
	    id;
	    
	    const
	    Data
	    chunkSize;
	    
	    const
	    uint
	    numChunks;

	private:
	    std::vector<Chunk*> chunks;

	    Action
	    createChunks();

	public:

	    TorrentFile(const File::File& a_f,
			const Data& a_chunkSize = DEF_CHUNKSIZE)
		: Files::File(f),
		  id(),
		  chunkSize(a_chunkSize),
		  chunks()
	    {
		createChunks();
	    }

	    TorrentFile(const char* a_name,
			const Data& a_size = DEF_SIZE,
			const Data& a_chunkSize = DEF_CHUNKSIZE)
		: Files::File(a_name,a_size),
		  id(),
		  chunkSize(a_chunkSize),
		  numChunks((a_size / a_chunkSize) + // round up to next highest integer if not 
			    (a_size.value() % a_chunkSize.value()) ? 1 : 0), // exactly even

	    {
		createChunks();
	    }
	};
    }
}
	    

#endif //  __ARCHITECTURES__BITTORRENT__TORRENTFILE_HXX__

