// CPP14Features.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <array>
#include <numeric>
#include <functional>
#include "UserLiterals.h"

auto budget_met = [](auto sales, auto budget) {
	return std::inner_product(std::begin(sales), std::end(sales),
		std::begin(budget),
		0,
		std::plus<decltype(0)>(),
		std::greater_equal<double>());
};

int func()
{
	int v = 42;
	return v;
}

int& func2()
{
	int v = 42;
	return v;
}

const double INIT_VALUE = 3.2;

class Init3 {
public:
	int first = { 1 };
	//float second = { INIT_VALUE }; // error C2397: conversion from 'const double' to 'float' requires a narrowing conversion
	float second = INIT_VALUE; //  warning C4244: 'initializing': conversion from 'const double' to 'float', possible loss of data
	std::string third = "Hello";
};

int main()
{

	CPP_REQUIRED(140); //Ensure C++ 14 is available

	printCPPStandardInUse();

	userLiterals();
	userLiterals2();

	auto && v1 = func();
	decltype(auto) v1_ = func();
	decltype(auto) v1_2 = (func());
	decltype(auto) _42 = 42;
	decltype(auto) __42 = (_42);
	auto v1_auto = func();

	cout << v1 << el;
	cout << v1_ << el;
	cout << v1_auto << el;

	auto && v2 = func2();
	decltype(auto) v2_ = func2();
	decltype(auto) v2_2 = (func2());
	auto v2_auto = func2();


	cout << v2 << el;
	cout << v2_ << el;
	cout << v2_auto << el;

	//See http://codexpert.ro/blog/2014/04/18/generic-numeric-algorithms-in-header-numeric/
	std::array<double, 12> budget = { 1000, 1500, 2000, 1500, 2000, 1000,
		3000, 1500, 1200, 1800, 2200, 2000 };

	std::array<double, 12> sales = { 1000, 1400, 2100, 2000, 1800,  900,
		3500, 1300, 1000, 2000, 2000, 2500 };

	decltype(auto) count = budget_met(sales, budget);


	//'calculando' el tipo de count2
	decltype(std::inner_product(std::begin(sales), std::end(sales), std::begin(budget), 0, std::plus<int>(),std::greater_equal<double>())) 
		count2 = std::inner_product(std::begin(sales), std::end(sales),
		std::begin(budget),
		0,
		std::plus<int>(),
		std::greater_equal<double>());

    return 0;
}

