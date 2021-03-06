// ModernCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ModernCPP.h"
#include "Lambdas.h"
#include <iostream>
#if CPP_VER > 98
#include <future>
#endif

//Lambdas with Y-combinator example
void LambdasYCombinator()
{
#if (CPP_VER > 110)
	([](auto f, auto n, auto m) {
		f(f, n, m);
	})(
		[](auto f, auto n, auto m) -> void
	{
		cout << typeid(n).name() << el;
		cout << n << el;
		if (n < m)
			f(f, ++n, m);
	},
		1, 20);


	([](auto f, auto n) {
		cout << "fibbonachi de " << n << " = " << f(f, n) << el;
	})(
		[](auto f, auto i) -> int
	{
		return (i <= 1u) ? i : (f(f, i - 1) + f(f, i - 2));
	},
		8);

	int sum1 = ([](auto f, auto n, auto m) {
		return f(f, n, m);
	})(
		[](auto f, auto n, auto m) -> int
	{
		if (n > m)
			return 0;
		else
			return n + f(f, n + 1, m);
	},
		1, 10);


	auto sum = [](auto n, auto m) {
		return ([](auto f, auto n, auto m)  {
			int res = f(f, n, m);
			return res;
		})(
			[](auto f, auto n, auto m) {
			if (n > m)
				return 0;
			else {
				int res = n + f(f, n + 1, m);
				return res;
			}
		},
			n, m); };

#if (CPP_VER > 140)
	//constexpr lambda C++17
	auto constSum = [](auto a, auto b)
		constexpr -> auto
	{
		return a + b;
	};

	
	constexpr auto sumConst = constSum(1, 5);
	static_assert(sumConst == 6, "not 6");
#endif
	auto result = sum(1, 10);

#endif
}


typedef void(*PFD)(double);    // C style -> PFD is the typename
#if CPP_VER > 98
using PF = void(*)(double);    // using plus C-style type
#endif


// Add good ways of doing strong typedefs with C++ > 11 if any...
// Looks like ,maybe via meta classes, earliest in C++ 20
// Potentially with C++ 11 see https://foonathan.net/blog/2016/10/19/strong-typedefs.html
#if CPP_VER > 98
enum class TypesEnum {
#else
enum TypesEnum {
#endif
	Length,
	Weigth
};

#if CPP_VER > 98 // Uses using from C++11
template <typename T, TypesEnum N> // N is used for tagging
struct strong_typedef
{
	using strong_type = strong_typedef<T, N>; // 'typedef' for the strong type
	T value; // the  wrapped value

	explicit strong_typedef(T val) : value(val) {}; // constructor
	strong_typedef() { value = {}; }; // default, zero-initialization

									  // operator overloading, basic example: 
	strong_type& operator+(const strong_type& rhs)
	{
		value += rhs.value;
		return *this;
	}
};

using Weight = strong_typedef<double, TypesEnum::Weigth>;
using Length = strong_typedef<double, TypesEnum::Length>;
#endif

void typedefStrong1()
{
#if CPP_VER > 98
	//Example

	//Still cumbersome, see user defined literals in C++ 14 
	//Example here in ../CPP14Features/UserLiterals.cpp

	Length width = Length{ 10 };
	Length height = Length{ 20 };
	Weight weight = Weight{ 5 };

	auto square = [](Length w, Length h) {
		return w + h;
	};

	//auto area = square(5, 5); //error C2664: ... cannot convert argument 1 from 'int' to 'Length'

	auto area = square(width, height); 

	//auto area2 = square(width, weight); //error C2664 : cannot convert argument 2 from 'Weight' to 'Length'
#endif
}



#if CPP_VER > 140
using namespace std;
using namespace std::chrono;

// this could be some long running computation or I/O
future<int> calculate_the_answer()
{
	return async([] {
		this_thread::sleep_for(1s); return 42;
	});
}

// Here is a resumable function
future<void> coro() {
	cout << "Started waiting..." << el;
	auto result = co_await calculate_the_answer();
	cout << "got " << result << el;
}

#endif

void coroutines() // Not yet standard, VC++ has something... with  /await compiler flag
{
#if CPP_VER > 140
	coro().get(); //Wait for future to resolve
#endif
	//keywords will be:
	//co_await;
	//co_return;
	//co_yield;
	
}



int main()
{
	printCPPStandardInUse();

	refToBadMemory();

	lambdas();

	LambdasYCombinator();

	typedefStrong1();

	//C++ 20?
	coroutines();

}

#pragma region RefToBadMemeory

//#pragma warning( error : 172 ) //Enable this
int& getRefToStack() {
	int stackValue = 123;

	//warning C4172 : returning address of local variable or temporary : stackValue
	return stackValue;  // DON'T DO THIS. 
}



void refToStackMemory()
{
	cout << "Running refToStackMemory samples\n";

	//Returning references to stack allocated memory
	//The pointed - to stack location stays stable as long as you don't make any function calls.
	int& gone = getRefToStack();
	auto autoRef = getRefToStack(); //copy not ref

	cout << "Value still    " << gone << '\n'; //One call and it's gone
	cout << "Random garbage " <<  gone << '\n'; //Ending with '\n' does not force flush the stream, like std::endl
	cout << std::endl; //Flushing the stream
}


struct simple
{
	simple() : i(345) {}
	int i;
};

//Returning reference to heap allocated memory
simple& getRefToHeap() {
	auto pI = new simple();
	return *pI;
}

void refToHeapMemory()
{
	cout << "Running refToHeapMemory samples\n";

	//Original reference lost, imposible to get to pointer and delete it
	simple noRef = getRefToHeap(); //LEAK
	cout << noRef.i << el;

	//Using auto doesnt give ref automatically
	auto autoNoRef = getRefToHeap(); //LEAK 
	cout << autoNoRef.i << el;

	auto& autoRef = getRefToHeap(); //Even with auto you need to a & aka ref in declaration
	cout << autoRef.i << el;

	//Have a look at auto -> CPP11Features: Auto.cpp

	simple& refInt = getRefToHeap();
	cout << refInt.i << el;

	//Weird gymnastics required to delete original heap pointer
	delete &autoRef;
	delete &refInt;

	//delete &autoNoRef; //Deleting a stack allocated copy !!!!

	// std::unique_ptr -> CPP11Features:  UniquePtr.cpp

	cout << refInt.i << el;
}


void refToBadMemory()
{
	refToStackMemory();
	refToHeapMemory();
}

#pragma endregion //RefToBadMemeory


