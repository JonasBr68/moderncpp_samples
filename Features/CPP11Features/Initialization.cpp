#include "stdafx.h"
#include "Initialization.h"
#include <string>
#include <initializer_list>
#include <stdint.h>

template<class T>
class Vector {
	std::vector<T> vectorData;
public:
	Vector(std::initializer_list<T> l):vectorData(l){
	}
	T get(int pos)
	{
		return vectorData.at(pos);
	}
};

void oldStyleListInitialization()
{
	//Via un array inicializado con una lista de inicializacion
	int carray[] = { 0,1,2,3,4 };

	std::vector<int> vec;
	for (int i = 0; i<5; i++) { vec.push_back(carray[i]); }

	std::set<int> conj;
	for (int i = 0; i<5; i++) { conj.insert(carray[i]); }

	std::map<int, std::string> mapa;
	mapa[0] = "cero";
	mapa[1] = "uno";
	mapa[2] = "dos";

	std::vector<int> vec2;
	vec2.push_back(10);
	vec2.push_back(20);
	vec2.push_back(30);
	vec2.push_back(40);
	int total = vec2.size();

	const char* values[] = { "Via", "a", "constructor", "using", "initializer_list" };
	std::vector<std::string> strings;
	for (int c = 0; c<5; c++) { strings.push_back(values[c]); }
}

void newCPP11Initialization()
{
#if CPP_VER > 98
	std::vector<int> vec = { 0,1,2,3,4 };

	std::set<int> conj = { 0,1,2,3,4 };

	std::map<int, std::string> mapa = { {0,"cero" }, {1, "uno"}, {2, "dos"} };
	auto second = mapa[2];
	cout << second << el;

	std::vector<std::string> strings = { "Via", "an", "initializer_list" };
	cout << strings.at(2) << el;

	int minusOne = -112345;

	//unsigned int ui { minusOne }; //error C2397: conversion from 'int' to 'unsigned int' requires a narrowing conversion
	unsigned int ui2 = minusOne;

	//int minusOne2{ ui2 }; //error C2397 : conversion from 'unsigned int' to 'int' requires a narrowing conversion
	int minusOne3 = ui2;


	double d = 3.2;
	float f = d;
	//float f2{ d }; //error C2397: conversion from 'double' to 'float' requires a narrowing conversion

	//No loss of data, no problem or warning
	double d2{ f };
	int64_t longLong{ minusOne };

#endif
}



struct TestStruct {
	int m_i;
	int m_x;
	double m_f;
};

void oldStructInitialization()
{
	TestStruct ts = { 1, 2.1, 3.2 }; // warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data

	TestStruct ts1 = { 1, 2 }; //Zero initializing the rest

	TestStruct ts2 = {};     // Initialize to 0
}

void newStructInitialization()
{
	TestStruct ts = { 1, 2.1, 3.2 }; // warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
	
	//TestStruct tsF2 = { 1, {2.1}, 3.2 }; // error C2397: conversion from 'double' to 'int' requires a narrowing conversion
#ifdef CPP11
	TestStruct* pTS = new TestStruct{ 1, 2, 3.2 }; 

	TestStruct* pTS1 = new TestStruct{ 1, 3};

	TestStruct* pTS2 = new TestStruct{ 1.2 , 2, 3.2 };  //warning C4244 : 'initializing' : conversion from 'double' to 'int', possible loss of data
	//TestStruct* pTS3 = new TestStruct{ {1.2}, 2, 3.2 }; // error C2440 : 'initializing' : cannot convert from 'initializer-list' to 'TestStruct'
#endif
}




class Test
{
	public:
		Test(int i, std::string tipo, double f = 0.0) :
			i(i),
			tipo(tipo),
			f(f)
			//Falta el p
		{

		}
		Test(int i) :
			i(1),
			f(f)
			//Que vamos hacer con el tipo?
		{
			p = f * i;
		}
		int getI() {
			return i; 
		}
		int p;
	private:
#ifdef CPP11
		int i{1};
		std::string tipo = "NoTipo";
		double f{3.14f};

		//int i = 1;
		//std::string tipo = "NoTipo";
		//double f = 3.14f;
#else
		int i;
		std::string tipo;
		float f;
#endif
};

#if CPP_VER > 98
class Test11
{
public:
	//delegating constructor
	Test11(int i, double f) : Test11(i, "Test11(int i, double f)", f) //Delegating to other constructor
	{
		p = f * i;
	}

	Test11(int i, std::string tipo, double f = 0.0) :
		i(i),
		tipo(tipo),
		f(f)
	{

	}
	
	int getI() {
		return i;
	}
	int p{ -1 };
private:
	int i{ 0 };
#ifdef CPP11
	std::string tipo = "Sin Tipo";
#else
	//Tecnically C++ 11 but not implemeneted in VS 2013, available from VS2015
	std::string tipo{ "Sin Tipo" }; //error C2797: 'Test11::tipo': list initialization inside member initializer list or non-static data member initializer is not implemented
#endif
	double f{ 1.0f };
};
#endif
class Simple {
public:
	int first;
	double second;
	std::string third;
};

const double INIT_VALUE = 3.2;

#ifdef CPP11
class Init1 {
public:
	int first = 1;
	float second = INIT_VALUE;
	std::string third = "Hellos";
};

class Init2 {
public:
	int first = { 1 };
	//float second = { { INIT_VALUE } }; // error C2397: conversion from 'const double' to 'float' requires a narrowing conversion
	float second = { INIT_VALUE }; // warning C4244: 'initializing' : conversion from 'const double' to 'float', possible loss of data
	std::string third = "Hellos";
};

#endif

void oldClassInitialization()
{
	//Via constructor only way
	Test t(23, "old");

	Simple s1 = { 1, 2.1, "Simple" }; //Same behaviour as struct, Simple is an 'aggregate' class

									  //Simple* ps1 = new Simple { 1, 2.1, "Simple" };



	TestStruct ts = { 1,2, 3.2 }; // For structs you could do this since C99
	TestStruct ts2 = {};     // Initialize to 0

							 //TestStruct* pTs = new TestStruct {}; //error C2143: syntax error : missing ';' before '{'

							 // Using designator like below from C99 was never included C++ 11
							 //TestStruct ts2 = { .m_f = 2.3,.m_i = 1,2 }; 

	cout << ts.m_f << el; //3.2
	cout << ts2.m_f << el; //0
}




void newClassInitialization()
{
#if CPP_VER > 98


	// No matching constructor - error C2440: '<function-style-cast>' : cannot convert from 'initializer-list' to 'Test'
	//auto t0 = Test{ 34, "Test", 22, 3 }; 

	// No matching constructor - error C2440: '<function-style-cast>' : cannot convert from 'initializer-list' to 'Test'
	//auto t1 = Test{ 34 }; 


	auto t2 = new Test(32.1, "32.1"); //warning C4244 : 'argument' : conversion from 'double' to 'int', possible loss of data

	//auto t3 = new Test({ 32.1 }, "32.1"); // error C2398: Element '1': conversion from 'double' to 'int' requires a narrowing conversion

	//auto t4 = Test{ 32.1 , "Test" }; //error C2398: Element '1': conversion from 'double' to 'int' requires a narrowing conversion

	//auto t0 = Test{ { 32.1 }, "Test" }; //error C2398: Element '1': conversion from 'double' to 'int' requires a narrowing conversion
	auto t5 = Test{ 32, "Test" };

	Test t6 = { 0, "Test2" };

	Test t7 { 32, "Test" };


	cout << t6.getI() << el;

	//Simple es una clase 'agregada', como un struct sin constructor
	auto s0 = Simple{
		0.9, //first -- VS 2017 error C2397: conversion from 'double' to 'int' requires a narrowing conversion
		2.2, //second
		"s0" //third
	};

	Simple s1 = {
		0.9, //first
		2.2, //second
		"s0" //third
	};

	//Simple s2 {
	//	{ 0.9 }, //first -- VS 2013 error C2397: conversion from 'double' to 'int' requires a narrowing conversion
	//	2.2, //second
	//	"s0" //third
	//};

	Test arrTests[] = { { 1,"1" },{ 2,"2" },{ 3,"3" } };

	std::vector<Test> classes = { { 1,"1" },{ 2,"2"}, { 3, "3.5" },{ 3 ,"3" } };
	std::vector<Test> classes2 = { { 1,"1" },{ 0 },{ 2,"2", 3.1415 },{ 3 ,"3" } };


#endif
}

void Initialization()
{
	//Old style list initialization
	oldStyleListInitialization();

	//Initialization lists for std containters
	newCPP11Initialization();

	oldStructInitialization();

	newStructInitialization();

	oldClassInitialization();

	//Class initialization with initialization lists
	newClassInitialization();
}
