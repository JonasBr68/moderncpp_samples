// CPP11Features.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Initialization.h"
#include "UniquePtr.h"
#include "EnforceUniquePtr.h"

int main()
{
	//CPP_REQUIRED(110); //Ensure compilig for C++11 only
	printCPPStandardInUse();

	Initialization();

	uniquePtr();

	enforceUniquePtr();
}

