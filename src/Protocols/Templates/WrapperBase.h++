#ifndef PROTOCOLS__WRAPPERBASE_HXX__
#define PROTOCOLS__WRAPPERBASE_HXX__

#include <map>

template<typename _P>
struct TWrapperBase
{
    virtual bool
    Bind(typename _P::SlotType w, TReceiver<Protocol>* r)
    { return this->Receivers_.insert(std::make_pair(t,r)).second; }
    
    virtual TReceiver<Protocol>*
    Bound(typename _P::SlotType w) const
    { return ((auto it = Receivers_.find(w)) == Receivers_.end()) ? NULL : it->second; }
    
    virtual void
    Unbind(typename Protocol::SlotType w)
    { Receivers.erase(w); }
	
    TWrapperBase() : Receivers_() {}
    virtual ~TWrapperBase() {}
    
protected:
    std::map<typename _P::SlotType, TReceiver<Protocol>*>
    Receivers_;
};

#endif // PROTOCOLS__WRAPPERBASE_HXX__
