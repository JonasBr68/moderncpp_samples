#include "stdafx.h"
#include "OverrideFinal.h"

#if CPP_VER > 98
class Base
{
public:
	virtual ~Base() //Either virtual destructor (unless perf/memory critical), or make class final
	{
	}

	virtual void overrideMe(double d)
	{
		cout << "Base overrideMe " << d << el;
	}
};

class DerivedA : public Base
{
public:
	using Base::overrideMe; //Includes Base::overrideMe in class for overload resolution
	
	void overrideMe(double d) final
	{
		cout << "DerivedA overrideMe " << d << el;
	}
};

class DerivedB final : public Base
{
public:
	using Base::overrideMe;

	virtual void overrideMe(float f)
	//virtual void overrideMe(float f) override //error C3668: 'DerivedB::overrideMe' : method with override specifier 'override' did not override any base class methods
	{
		cout << "DerivedB overrideMe " << f << el;
	}
};

class DerivedC : public Base
{
	void overrideMe(double d) override {
		cout << "DerivedC overrideMe " << d << el;
	}
};

class DerivedDerivedC : public DerivedC
{
	void overrideMe(double d) override {
		cout << "DerivedDerivedC overrideMe " << d << el;
	}

};
//class DerivedDerivedB : public DerivedB //error C3246: 'DerivedDerivedB' : cannot inherit from 'DerivedB' as it has been declared as 'final'
//{
//
//};

class DerivedDerivedA : public DerivedA
{
public:
	DerivedDerivedA() {
		cout << "DerivedDerivedA constructor" << el;
	}
	virtual ~DerivedDerivedA() //Either virtual destructor, or make class final
	{
		cout << "DerivedDerivedA destructor" << el;
	}


	//void overrideMe(double d) //error C3248: 'DerivedA::overrideMe': function declared as 'final' cannot be overridden by 'DerivedDerivedA::overrideMe'
	//{
	//	cout << "DerivedDerivedA overrideMe()" << el;
	//}

};
#endif

void overrideFinal()
{
#if CPP_VER > 98
	Base* pA = new DerivedA();
	Base* pB = new DerivedB();

	pA->overrideMe(2.2); //calls DerivedA
	pB->overrideMe(2.2); //calls Base
	pB->overrideMe(2.2f); //calls Base
	static_cast<DerivedB*>(pB)->overrideMe(2.2); //calls Base due to the using Base::overrideMe; statement inlcuding it in overload resolution
	static_cast<DerivedB*>(pB)->overrideMe(2.2f); //calls DerivedB

	Base* pDDC = new DerivedDerivedC();
	pDDC->overrideMe(3.14); //Calls DerivedDerivedC
#endif
}