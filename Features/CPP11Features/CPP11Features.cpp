// CPP11Features.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Auto.h"
#include "Initialization.h"
#include "ConstExpr.h"
#include "UniquePtr.h"
#include "EnforceUniquePtr.h"
#include "SharedPtr.h"
#include "MultiThreading.h"
#include "StrongEnum.h"
#include "PlacementNew.h"
#include "OverrideFinal.h"
#include "FunctorStruct.h"
#include "TrailingReturnType.h"
#include "ConditionVariables.h"
#include "Threads.h"
//#include <iostream>

int main()
{
	//CPP_REQUIRED(110); //Ensure compilig for C++11 only
	printCPPStandardInUse();

	trailingReturns();

	functorAccumulate();

	manualLambda();

	withLambda();

	strongEnums();

#if CPP_VER > 98
	auto wstr = LR"(wchar_t No escaping here\n \\)";
	cout << R"(No escaping here\n \\)" << el; //Raw string

	//Not recommended to switch/use both cout and wcout
	std::wcout << LR"(wchar_t No escaping here\n \\)" << el; //Raw wchar_t string

	cout << "stdout coprrupted?" << el;
#endif

	autoConstAndRef();

	uniquePtr();

	enforceUniquePtr();

	sharedPtrCyclical();

	sharedPtrRefCounting();

	Initialization();

	overrideFinal();

	constExpr();


	placementNew();

	callOnce();

	atomicSingleton();

	threads();

	safeCounter();


	alternateThreads();

	startThreadAndSignal();

	stopStartThreads();
	conditionVariables();
}

