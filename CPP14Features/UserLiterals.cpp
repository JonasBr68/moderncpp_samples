#include "stdafx.h"
#include "UserLiterals.h"
#include <bitset>
#include <iostream>
#include <vector>


using myvec = std::vector<int>;

#if CPP_VER > 110
template<int M, int K, int S> struct Unit { // a unit in the MKS system
	enum class MKSUnit { m = M, kg = K, s = S };
};

template<typename Unit> // a magnitude with a unit
struct Value {
	double val; // the magnitude
	constexpr explicit Value(double d) : val(d) {} // construct a Value from a double
};

using Meter = Unit<1, 0, 0>; // unit: meter
using Second = Unit<0, 0, 1>; // unit: sec
using Speed = Value< Unit<1, 0, -1> >; // meters/second type

constexpr Value<Second> operator "" _s(long double d)
// a f-p literal suffixed by ‘_s’
{
	return Value<Second>(d);
}
constexpr Value<Meter> operator "" _m(long double d)
// a f-p literal suffixed by ‘_m’
{
	return Value<Meter>(d);
}

template<int m1, int k1, int s1, int m2, int k2, int s2>
Value<Unit<m1 - m2, k1 - k2, s1 - s2>> operator / (Value<Unit<m1, k1, s1>> a, Value<Unit<m2, k2, s2>> b)
{
	return Value<Unit<m1 - m2, k1 - k2, s1 - s2>>(a.val / b.val);
}
#endif



void userLiterals() {
#if CPP_VER > 110
	Speed sp1 = 100.0_m / 9.8_s;
	cout << sp1.val << el;
#endif

}




//Example from https://stackoverflow.com/questions/237804/what-new-capabilities-do-user-defined-literals-add-to-c#answer-7906630
template<char... Bits>
struct checkbits
{
	static const bool valid = false;
};

template<char High, char... Bits>
struct checkbits<High, Bits...>
{
	static const bool valid = (High == '0' || High == '1')
		&& checkbits<Bits...>::valid;
};

template<char High>
struct checkbits<High>
{
	static const bool valid = (High == '0' || High == '1');
};

template<char... Bits>
inline constexpr std::bitset<sizeof...(Bits)>
operator"" _bits() noexcept
{
	static_assert(checkbits<Bits...>::valid, "invalid digit in binary string");
	//return std::bitset<sizeof...(Bits)>((char[]) { Bits..., '\0' });
	std::string bits = std::string{ Bits... };
	return std::bitset<sizeof...(Bits)>(bits);
}

void userLiterals2() {
	auto bits = 0101010101010101010101010101010101010101010101010101010101010101_bits;
	std::cout << bits << std::endl;
	std::cout << "size = " << bits.size() << std::endl;
	std::cout << "count = " << bits.count() << std::endl;
	std::cout << "value = " << bits.to_ullong() << std::endl;

	//  This triggers the static_assert at compile time.
	//auto badbits = 2101010101010101010101010101010101010101010101010101010101010101_bits;

	//  This throws at run time.
	try {
		std::bitset<64> badbits2("2101010101010101010101010101010101010101010101010101010101010101_bits");
	}
	catch (std::invalid_argument& inv)
	{
		cout << inv.what() << el;
	}
}