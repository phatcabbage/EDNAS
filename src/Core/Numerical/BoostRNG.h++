#ifndef CORE__NUMERICAL__BOOSTRNG_HXX__
#define CORE__NUMERICAL__BOOSTRNG_HXX__

#ifdef USE_BOOST_
#include <boost/random.hpp>

#include "./RNG.h++"
namespace Core
{
    namespace Numerical
    {
	template <typename _R, typename _E, typename _D>
	class BoostRNG : public RNG<_R>
	{
	public:
	    typedef _R ResultType;
	    typedef _E EngineType;
	    typedef _D DistributionType;

	    typedef
	    boost::variate_generator<EngineType,DistributionType>
	    GeneratorType;

	private:
	    GeneratorType
	    Generator_;

	public:
	    BoostRNG(unsigned long seed=0)
		: RNG<_R>(seed)
		, Generator_(GeneratorType(EngineType(seed), DistributionType())) {}

	    virtual
	    ~BoostRNG() {}

	    ResultType
	    operator()() { return Generator_();}
	};

	template<typename _R, typename _E>
	class BoostRNG<_R, _E, boost::poisson_distribution<> >
	{
	public:
	    typedef _R ResultType;
	    typedef _E EngineType;

	    typedef
	    boost::poisson_distribution<>
	    DistributionType;

	    typedef
	    boost::variate_generator<EngineType,DistributionType>
	    GeneratorType;
    
	private:
	    GeneratorType
	    Generator_;

	    double
	    Lambda_;

	public:
	    BoostRNG(double lambda = 1.0f, unsigned long seed = 0)
		: RNG<_R>(0)
		, Generator_(EngineType(seed), DistributionType(lambda))
		, Lambda_(lambda)
	    {}

	    virtual
	    ~BoostRNG() {}

	    ResultType
	    operator()() { return Generator_();}
	};
	      
	typedef BoostRNG<int, boost::lagged_fibonacci607,boost::poisson_distribution<> > PoissonRNG;
    }
}
#endif // USE_BOOST_
#endif // CORE__NUMERICAL__BOOSTRNG_HXX__
