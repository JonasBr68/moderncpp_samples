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
	auto i1 = getInt();		//auto does not reflect const
	auto i2 = getIntRef();	//auto does not reflect reference &

	const auto i3 = getInt();
	const auto& i4 = getIntRef();
	const auto i5 = getIntRef();


	cout << i1 << " " << i2 << el;
}
