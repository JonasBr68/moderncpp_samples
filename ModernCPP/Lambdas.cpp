#include "stdafx.h"
#include "Lambdas.h"


//A lambda with no state (captures/closure) can be demoted to a function pointer
void callLambda(int n, int m, void (*func)(int))
{
	func(n);
	if (n < m)
		callLambda(++n, m, func);
}

void callStdLambda(int n, int m, std::function<void (int)> func)
{
	func(n);
	if (n < m)
		callStdLambda(++n, m, func);
}
#ifdef CPP14
auto magicLambda = [](auto n) {
	cout << typeid(n).name() << el;
	cout << n << el;
};
#endif

#ifdef CPP14
template<class... T>
double sum2(T... t)
{
	double result{};

	auto add = [&result](double elem)
	{
		result += elem;
	};

	(add(t), ...);

	return result;
}

template<typename ...Args>
void print(Args&&... args) {
	(std::cout << ... << args) << '\n';
}

auto printAll = [](auto... t) {
	(cout << ... << t);
};


auto sumAll = [](auto... n) {
	return (n + ...);
};
#endif

void lambdas() {

	//printAll("Hello ", "world ", 1, 2, 3);
	//cout << el;

	//auto res = sum2(1, 2, 3, 4, 5, 2.3, 2.1, 2, 3);
	//auto all = sumAll(1, 2, 3, 4, 5, 2.3, 2.1, 2, 3);

#ifdef CPP11
	callLambda(1, 20,
		[](int n)-> void { 
		cout << /*msg <<*/ n << el;
	});
#endif
	auto msg = "Current number ";
	//Will not compile as lambda with capture cannot be demoted to function pointer, needs std::function
	//callLambda(1, 20,
	//	[msg](int n)-> void { // error C2664: 'void callLambda(int,int,void (__cdecl *)(int))': cannot convert argument 3 to 'void (__cdecl *)(int)'
	//	cout << msg << n << el;
	//});

	callStdLambda(1, 20,
		[msg](int n)-> void {
		cout << msg << n << el;
	});

#ifdef CPP14
	//Generic lambda
	magicLambda("Hola lambda generico");
	magicLambda(12345);

	([](auto n) {
		cout << typeid(n).name() << el;
		cout << n << el;
	})("hi");
#endif

}

