#ifndef PROTOCOLS__ADDRESS_HXX__
#define PROTOCOLS__ADDRESS_HXX__

template<typename T>
struct TAddress
{
    int Value;
    TAddress() = default;
    
    TAddress(const TAddress& t)
	: Value(t.Value)
    {}

    operator const int&() const { return Value; }
    operator int&() { return Value; }
    
    TAddress(int i) : Value(i) {}
    
    TAddress&
    operator=(const TAddress& t)
    { Value = t.Value; return *this;}
    
    TAddress&
    operator=(int i)
    { Value = i; return *this; }
    
    TAddress&
    operator+=(const TAddress& t)
    { Value += t.Value; return *this;}
    
    bool
    operator==(const TAddress& t) const
    { return Value == t.Value; }

    bool
    operator!=(const TAddress& t) const
    { return Value != t.Value; }

    bool
    operator<(const TAddress& t) const
    { return Value < t.Value; }
	
    bool
    operator<=(const TAddress& t) const
    { return Value <= t.Value; }
	
    bool
    operator>(const TAddress& t) const
    { return Value > t.Value; }

    bool
    operator>=(const TAddress& t) const
    { return Value >= t.Value; }
	
    operator bool() const
    { return Value != 0; }
	
    bool
    operator==(int i) const
    { return Value == i; }

    bool
    operator!=(int i) const
    { return Value != i; }

    bool
    operator<(int i) const
    { return Value < i; }

    bool
    operator!() const
    { return !Value; }

    TAddress&
    operator++()
    { ++Value; return *this; }

    TAddress
    operator++(int)
    { TAddress t(Value); ++Value; return t; }
	
    uint16_t
    Byte(int i) const
    { return (static_cast<uint16_t>(Value >> (i * 8)) & 0xFF); }
};

template<typename T>
int
operator+(const TAddress<T>& a, const TAddress<T>& b)
{ return a.Value + b.Value; }


template<typename T>
int
operator&(const TAddress<T>& a, const TAddress<T>& b)
{ return a.Value & b.Value; }


#endif // PROTOCOLS__ADDRESS_HXX__
