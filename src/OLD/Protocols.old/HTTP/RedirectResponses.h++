#ifndef PROTOCOLS__HTTP__REDIRECTRESPONSES_HXX__
#define PROTOCOLS__HTTP__REDIRECTRESPONSES_HXX__

#include "./Response.h++"

namespace Protocols
{
    namespace HTTP
    {
	template<>
	struct TStatusResponse<Response::REDIRECT> : public Response
	{
	    const URL& 
	    RedirectURI;
	    
	    virtual ~TStatusResponse() {}
	protected:
	    TStatusResponse(const Response::ResponseValue& value,
			    const URL& uri,
			    const URL& redirectURI)
		: Response(Response::REDIRECT, value, uri)
		, RedirectURI(redirectURI) {}
	};

	typedef TStatusResponse<Response::REDIRECT> RedirectResponse;
	
	template<Response::ResponseValue _V>
	struct TRedirectResponse : public RedirectResponse
	{
	    
	    virtual ~TRedirectResponse() {}
	    TRedirectResponse(const URL& uri, const URL& redirectURI) 
		: RedirectResponse(_V, uri, redirectURI) {}
	};

	typedef TRedirectResponse<Response::MOVEDPERMANENTLY> MovedPermanently;
	typedef TRedirectResponse<Response::FOUND>            Found;
	typedef TRedirectResponse<Response::SEEOTHER>         SeeOther;
    }
}

#endif // PROTOCOLS__HTTP__REDIRECTRESPONSES_HXX__
