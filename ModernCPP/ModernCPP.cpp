// ModernCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ModernCPP.h"
#include "Lambdas.h"

#ifdef CPP11
enum class Topic
#else
enum Topic
#endif
{
	RefToBadMemeory,
	Lambdas,
	UniquePtr1,
};


//void printCPPStandardInUse()
//{
//	cout << "_MSC_VER: " << _MSC_VER << el;
//	cout << "_MSC_FULL_VER: " << _MSC_FULL_VER << el;
//#ifdef _MSVC_LANG
//	cout << "_MSVC_LANG: " << _MSVC_LANG << el;
//#endif
//	cout << "Project compiled with ";
//	cout << "VS Version " << VSVERSTR << el;
//	cout << CPPVERSTR << el;
//
//}

void YCombinator()
{
#if (0 && CPP_VER > 98)
	([](auto f, auto n, auto m) {
		f(f, n, m);
	})(
		[](auto f, auto n, auto m) -> void
	{
		cout << typeid(n).name() << el;
		cout << n << el;
		if (n<m)
			f(f, ++n, m);
	},
	1, 20);

	//int sum = ([](auto f, auto n, auto m) {
	//	return f(f, n, m);
	//})(
	//	[](auto f, auto n, auto m) -> int
	//{
	//	if (n > m)
	//		return 0;
	//	else
	//		return n + f(f, n + 1, m);
	//},
	//	1, 10);

	auto sum = [](auto n, auto m) {
		return ([](auto f, auto n, auto m) {
			int res = f(f, n, m);
			return res;
		})(
			[](auto f, auto n, auto m) -> int
			{
				if (n > m)
					return 0;
				else {
					int sum = n + f(f, n + 1, m);
					return sum;
				}
			},
			n, m); };

	auto result = sum(1, 10);

	//if (a>b)
	//	return 0;
	//else
	//	return term(a) + sum(next(a), b);

	//int main() {
	//	std::cout << sum(1, 10) << std::endl;
	//	return 0;
	//}


	//JavaScript
	//((f) = > {
	//	f(1, 20, f);
	//})((n, m, f) = > {
	//	console.log(n);
	//	if (n<m)
	//		f(++n, m, f);
	//});
#endif
}


//Add good ways of doing strong typedefs with C++ > 11 if any...
void typedefStrong()
{

}

void coroutines()
{
	//co_await;
	//co_return;
	//co_yield;
	
}

int main()
{
	printCPPStandardInUse();
	//YCombinator();
	typedefStrong();
	auto topic = Topic::Lambdas;

	switch (topic)
	{
	case Topic::RefToBadMemeory:
		refToBadMemory();
		break;
	case Topic::Lambdas:
		lambdas();
		break;
	default:
		break;
	}
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
	auto autoRef = getRefToStack();

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


	simple& refInt = getRefToHeap();
	cout << refInt.i << el;

	//Weird gymnastics required to delete original heap pointer
	delete &autoRef;
	delete &refInt;
	delete &autoNoRef; //Deleting a stack allocated copy !!!!

	cout << refInt.i << el;
}


void refToBadMemory()
{
	cout << "Running refToBadMemeory illustrations:\n\n";

	refToStackMemory();
	refToHeapMemory();
}

#pragma endregion //RefToBadMemeory


