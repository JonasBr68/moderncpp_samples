#pragma once

#include <iostream>
#include <yvals.h>
using std::cout;

#define el '\n'


#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define VSVERSTR STR(_MSC_VER)

#ifdef _MSVC_LANG
#if (_MSVC_LANG == 201402L)
#define CPPVERSTR "/std:c++14"
#define CPP_VER 140
#elif (_MSVC_LANG == 201703L)
#define CPPVERSTR "/std:c++17"
#define CPP_VER 170
#else
#define CPPVERSTR "/std:c++latest"
#define CPP_VER 180
#endif
#else
#if (_MSC_VER > 1600) //If later than VS 2010
#define CPP11
#define CPP_VER 110
#define CPPVERSTR "C++11"
#else
#define CPPVERSTR "C++98"
#define CPP_VER 98
#endif
#endif
#if (_MSC_VER > 1900 && _MSC_VER <= 1913)
#define VS_PROD "Visual Studio 2017"
#elif (_MSC_VER == 1900)
#define VS_PROD "Visual Studio 2015"
#elif (_MSC_VER == 1800)
#define VS_PROD "Visual Studio 2013"
#elif (_MSC_VER == 1700)
#define VS_PROD "Visual Studio 2012"
#elif (_MSC_VER == 1600)
#define VS_PROD "Visual Studio 2010"
#else
#define VS_PROD "Visual Studio Unknown"
static_assert(_MSC_VER > 1599 && _MSC_VER < 1914, "Failed detection of VS");
#endif

//MSVC++ 14.1 _MSC_VER == 1913 (Visual Studio 2017)
//MSVC++ 14.1 _MSC_VER == 1912 (Visual Studio 2017)
//MSVC++ 14.1 _MSC_VER == 1911 (Visual Studio 2017)
//MSVC++ 14.1 _MSC_VER == 1910 (Visual Studio 2017)
//MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
//MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
//MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
//MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
//MSVC++ 9.0  _MSC_FULL_VER == 150030729 (Visual Studio 2008, SP1)
//MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
//MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
//MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
//MSVC++ 7.0  _MSC_VER == 1300
//MSVC++ 6.0  _MSC_VER == 1200
//MSVC++ 5.0  _MSC_VER == 1100
#define CPP_REQUIRED(level) static_assert(level == CPP_VER, "Not correct CPP level configured in project")

inline void printCPPStandardInUse()
{
	cout << "_MSC_VER: " << _MSC_VER << el;
#ifdef _MSVC_LANG
	cout << "_MSVC_LANG: " << _MSVC_LANG << el;
#endif
	cout << "_CPPLIB_VER " <<  _CPPLIB_VER << el;
	cout << "Project compiled with ";
	cout << VS_PROD << " toolset" << el;
	cout << CPPVERSTR << el;
	cout << "CPP Version: " << CPP_VER << el;
}


