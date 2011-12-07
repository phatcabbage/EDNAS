#ifndef CORE__NUMERICAL__RNG_HXX__
#define CORE__NUMERICAL__RNG_HXX__

namespace Core
{
    namespace Numerical
    {
	template<typename _Result>
	class RNG
	{
	private:
	    long int
	    Seed_;
    
	public:
	    typedef _Result ResultType;
	
	    explicit 
	    RNG(const long seed)
		: Seed_(seed) {}

	    virtual
	    ~RNG() {}

	    long int
	    GetSeed() const
	    { return this->Seed_; }

	    void
	    SetSeed(const long seed)
	    { Seed_ = seed; }

	    virtual void
	    SeedRNG() {}

	    virtual ResultType
	    operator()() = 0;

	protected:
	    RNG(const RNG&);
	};
    }
}
#endif // RANDOM__RNG_HXX__
