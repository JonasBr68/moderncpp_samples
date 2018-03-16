#include "stdafx.h"
#include "TrailingReturnType.h"

//Trailing Return Type, new syntax

//OLD
int f1(double, int i) {
	return i;
}

auto f2(double, int a) -> decltype(a)
{
	return a;
}

template <typename A, typename B>
auto f3(A, B a) -> decltype(a) // sabemos es B asi aqui no tiene mucho sentido aqui...
{
	return a;
}

template <typename A, typename B>
B f4(A, B a)
{
	return a;
}

#if CPP_VER > 110
constexpr size_t maxsize(size_t a, size_t b)
{
	return (a > b) ? a: b;
}
#endif
template < typename T1, typename T2 >
auto multiply(T1 t1, T2 t2) -> decltype(t1*t2) //Usaba decltype(T1*T2) y no compilaba
{
#if CPP_VER > 110
	constexpr size_t max = maxsize(sizeof(T1), sizeof(T2));
	constexpr size_t result_size = sizeof(decltype(t1*t2));
	static_assert(result_size >= max, "Loss of data");
	//static_assert(result_size < max, "Loss of data");
	cout << result_size << el;
#endif
	return t1 * t2;
}

//Tampoco correcto
template < typename T1, typename T2 >
decltype(T1*T2) multiply2(T1 t1, T2 t2)
{
	return t1 * t2;
}

#if CPP_VER > 110 //Generic lambda C++14
auto multiplyLambda = [](auto a, auto b)
{
	return a + b;
};
#endif

void trailingReturns() {
	auto r2 = f2(2.0, 1);
	auto r3 = f3(2.0, 1);
	auto r4 = f4(2.0, 1);

	//NO FUNCIONABA!!! Sin problema con lambda generica
	auto p1 = multiply(2.3f, 3.0);
	auto p2 = multiply<float, double>(2.3f, 3.0);
	auto p3 = multiply(2.3f, 3);
	//auto p3 = multiply2<float, double>(2.3f, 3.0);
	//auto p4 = multiply2<float, double>(2.3f, 3.0);

#if CPP_VER > 110 //Generic lambda C++14
	//Generic lambda es la leche
	auto p5 = multiplyLambda(2.3f, 3.0);
#endif
}


