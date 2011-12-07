#ifndef CORE__NUMERICAL__NUMBERGENERATOR_HXX
#define CORE__NUMERICAL__NUMBERGENERATOR_HXX

namespace Core
{
    namespace Numerical
    {
	template<typename _Result>
	struct NumberGenerator
	{
	public:
	    typedef _Result ResultType;
	    typedef _Result result_type; // for Boost compatibility

	    virtual ResultType
	    operator()() = 0;
	};

	template<typename _R, typename _NG>
	class RangeAdapter
	{
	public:
	    typedef _R ResultType;
	    typedef _NG NumberGeneratorType;

	private:
	    NumberGeneratorType 
	    Generator_;
	
	    ResultType Min_, Max_;

	public:
	    RangeAdapter(ResultType min,
			 ResultType max,
			 const _NG& gen = _NG())
		: Min_(min)
		, Max_(max)
		, Generator_(gen) {}

	    virtual ~RangeAdapter() {}

	    void SetMin(ResultType min) { Min_ = min; }
	    void SetMax(ResultType max) { Max_ = max; }
	    ResultType GetMin() const   { return Min_;}
	    ResultType GetMax() const   { return Max_;}

	    ResultType
	    operator()() { return (Min_ + Generator_() * (Max_ - Min_)); }
	};
    }
}

#endif // CORE__NUMERICAL__NUMBERGENERATOR_HXX
