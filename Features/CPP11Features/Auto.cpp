#include "stdafx.h"
#include "Auto.h"


const int getInt()
{
	return 9;
}

const int& getIntRef()
{
	return 99;
}

void autoConstAndRef()
{
	auto i1 = getInt();
	auto i2 = getIntRef();
	cout << i1 << " " << i2 << el;
}
