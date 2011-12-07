#ifndef SCENARIOS__WEBSERVER__RESULTSPRINTER_HXX__
#define SCENARIOS__WEBSERVER__RESULTSPRINTER_HXX__

namespace Scenarios
{
    namespace Webserver
    {
	struct ResultsPrinter 
	{
	public:
	    void operator()() const;
	private:
	    void Description() const;
	    void ClientTimes() const;
	};
    }
}

#endif // SCENARIOS__WEBSERVER__RESULTSPRINTER_HXX__
