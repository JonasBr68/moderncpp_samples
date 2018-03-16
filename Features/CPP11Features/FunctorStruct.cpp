#include "stdafx.h"
#include "FunctorStruct.h"

struct Sum
{
	//Call operator
	Sum& operator()(int a) {
		val += a;
		return *this;
	}
	int val{};
};

void functorAccumulate()
{
	Sum acc;

	acc(1)(2)(3)(4)(5)(6)(7)(8)(9)(10);

	cout << acc.val << el;

	Sum acc2;
	for (int i = 1; i <= 10; i++) {
		acc2(i);
	}
	cout << acc2.val << el;
}

struct OneClosureByRef
{
	OneClosureByRef(int& a):m_a{a}
	{}
	int& m_a;
	int operator()(int b) {
		m_a += b;
		return m_a;
	}

};

void manualLambda() {
	int acc = 1;
	auto functorStruct1 = OneClosureByRef(acc);
	for (int i = 2; i < 10; ++i)
	{
		functorStruct1(i);
	}
	cout << acc << el;
	cout << functorStruct1(0) << el;
}


//declval is commonly used in templates where acceptable template parameters may have no constructor in common, 
//but have the same member function whose return type is needed
//decltype(std::declval<NonDefault>().foo())

template <class T>
class OneClosureByRefT
{
public:
	OneClosureByRefT(T& a) : m_a{ a }
	{}
	T& m_a;
	T operator()(T b) {
		m_a += b;
		return m_a;
	}

};

//Change acc to double, float etc the type 'flows' through
void manualLambdaTemplate() {
	auto acc = 1.1f; 
	auto manualLambda = OneClosureByRefT<decltype(acc)>(acc); //See also std::declval
	for (auto i = acc + 1; i < 10; ++i)
	{
		manualLambda(i);
	}
	cout << acc << el;
	cout << manualLambda(0) << el;
}

//Change acc to double, float etc the type 'flows' through
void withLambda() {
	auto acc = 1;

	//In C++14, generic lambdas where introduced which allow the usage of auto.
#if CPP_VER > 110
	auto lambda1 = [&acc](auto a) { return acc += a; }; // Capture by reference
#else
	auto lambda1 = [&acc](decltype(acc) a) { return acc += a; }; // Capture by reference
#endif
	for (auto i = acc + 1; i < 10; ++i)
	{
		lambda1(i);
	}
	cout << acc << el;
	cout << lambda1(0) << el;
}

//Generic lambda, C++14
#if CPP_VER > 110
auto f2lambda = [](double, auto a) -> auto
{
	return a;
};
#endif

//Luckily, in C++14 we got return type deduction for functions, so we can leave it to the compiler and remove this use of decltype.
template <class T, class U>
auto add(T const& t, U const& u) -> decltype(t + u) {
	return t + u;
}

template <class T, class U>
auto add2(T const& t, U const& u) -> T {
	return t + u;
}



struct Default { int foo() const { return 1; } };

struct NonDefault
{
	NonDefault(const NonDefault&) { }
	int foo() const { return 1; }
};


// https://arne-mertz.de/2017/01/decltype-declval/

void declVal()
{

	decltype(Default().foo()) n1 = 1;                   // type of n1 is int
														//  decltype(NonDefault().foo()) n2 = n1;               // error: no default constructor
	decltype(std::declval<NonDefault>().foo()) n2 = n1; // type of n2 is int

#if CPP_VER > 110 // decltype(auto) new in C++14
	decltype(auto) n3 = n1; // type of n2 is int
#endif
	auto n4 = n1; // type of n2 is int
	std::cout << "n1 = " << n1 << '\n'
		<< "n2 = " << n2 << '\n';
}
