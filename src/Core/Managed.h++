#define CORE__MANAGED_HXX__
#ifndef CORE__MANAGED_HXX__
#define CORE__MANAGED_HXX__

#include <set>

struct Managed
{
    static size_t 
    Count();

protected:
    Managed()
    { Managed::Add(this); }
    
    virtual 
    ~Managed()
    { Managed::Remove(this); }

private:
    static void 
    Add(Managed* m);
    
    static void
    Remove(Managed* m);

    struct Manager
    {
	std::set<Managed*> Objects;
	Manager() : Objects(){}
	~Manager() { while (!Objects.empty()) delete *(Objects.begin()); }
	
	Manager&
	operator+=(Managed* m)
	{ Objects.insert(m); return *this; }

	Manager&
	operator-=(Managed* m)
	{ Objects.erase(m); return *this; }
    };
    static Manager Mgr;
};


#endif // CORE__MANAGED_HXX__
