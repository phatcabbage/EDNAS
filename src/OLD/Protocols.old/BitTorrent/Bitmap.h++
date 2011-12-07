#ifndef PROTOCOLS__BITTORRENT__BITMAP_HXX__
#define PROTOCOLS__BITTORRENT__BITMAP_HXX__

#include <vector>

namespace Protocols
{
    namespace BitTorrent
    {
	struct Bitmap
	{
	    virtual const bool&
	    operator[](int) const = 0;

	    virtual bool&
	    operator[](int) = 0;

	    virtual int
	    Size() const = 0;

	    static
	    Bitmap*
	    Clone(const Bitmap& b);
	};	    

	template<typename _Backing>
	class TBitmap : public Bitmap
	{
	public:
	    typedef _Backing BackType;
	private:
	    BackType Back_;

	public:
	    explicit 
	    TBitmap(const TBitmap& other) : Back_(other.Back_) {}
	    
	    explicit
	    TBitmap(const int size) : Back_(size) {}

	    const bool&
	    operator[](int) const
	    { return Back_[int];}

	    bool&
	    operator[](int)
	    { return Back_[int];}

	    int
	    Size() { return Back_.size();}
	};

	typedef TBitmap<std::vector<bool> > VectorBitmap;
    }
}
#endif // PROTOCOLS__BITTORRENT__BITMAP_HXX__
