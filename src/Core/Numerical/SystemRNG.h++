#ifndef CORE__NUMERICAL__SYSTEMRNG_HXX__
#define CORE__NUMERICAL__SYSTEMRNG_HXX__

#include "./RNG.h++"
#include <cstdlib> // for srand48_r() and malloc()
#include <ctime> // for time(NULL)  (for srand())

namespace Core
{
    namespace Numerical
    {
	template<typename _Result>
	class SystemRNG : public RNG<_Result>
	{
	private:
	    struct drand48_data* 
	    State_;

	    void
	    Init(bool seedWithSys);

	public:
	    typedef _Result ResultType;

	    explicit
	    SystemRNG()
		: RNG<_Result>(0)
		, State_(0) 
	    { this->Init(true);}

	    explicit
	    SystemRNG(const long seed)
		: RNG<_Result>(seed) 
		, State_(0)
	    { this->Init(false);}

	    virtual void
	    SeedRNG();

	    void
	    SeedWithSystemClock();

	    virtual 
	    ~SystemRNG();

	    virtual ResultType
	    operator()();
	};

	template<typename R>
	void
	SystemRNG<R>::
	Init(bool seedWithSys)
	{
	    if (!this->State_)
		this->State_ = 
		    static_cast<drand48_data*>(malloc(sizeof(drand48_data)));
	
	    if(seedWithSys)
		this->SeedWithSystemClock();
	    else
		this->SeedRNG();
	}

	template<typename R>
	void
	SystemRNG<R>::
	SeedRNG()
	{
	    long seed = RNG<R>::GetSeed();
	    srand48_r(seed, this->State_);
	}

	template<typename R>
	void
	SystemRNG<R>::
	SeedWithSystemClock()
	{
	    long seed = time(NULL);
	    RNG<R>::SetSeed(seed);
	    this->SeedRNG();
	}

	template<typename R>
	SystemRNG<R>::
	~SystemRNG()
	{
	    free(State_);
	}

	template<>
	double
	SystemRNG<double>::
	operator()()
	{
	    double result;
	    drand48_r(this->State_, &result);
	    return result;
	}

	template<>
	long
	SystemRNG<long>::
	operator()()
	{
	    long result;
	    mrand48_r(this->State_, &result);
	    return result;
	}

	template<>
	int
	SystemRNG<int>::
	operator()()
	{
	    long result;
	    mrand48_r(this->State_, &result);
	    return (int)result;
	}

	template<>
	unsigned long
	SystemRNG<unsigned long>::
	operator()()
	{
	    unsigned long result;
	    lrand48_r(this->State_, (long*)&result);
	    return result;
	}

	template<>
	unsigned int
	SystemRNG<unsigned int>::
	operator()()
	{
	    unsigned long result;
	    lrand48_r(this->State_, (long*)&result);
	    return (unsigned int)result;
	}
    }
}
#endif // CORE__NUMERICAL__SYSTEMRNG_HXX__
