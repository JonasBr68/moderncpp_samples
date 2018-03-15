#include "stdafx.h"
#include "UniquePtr.h"
#include <memory>
#include <string>

#if CPP_VER > 98
class UPTest {
public:
	UPTest(int id):
		_id(id)
	{ }
	~UPTest() {
		cout << "deleted UPTest " << _id << el;
	}
	int _id;
};

std::unique_ptr<UPTest> getSomething() {
	static int c = 1;
	return std::make_unique<UPTest>(c++);
}

std::unique_ptr<UPTest> getAnotherOne()
{
	std::unique_ptr<UPTest> x = getSomething();
	return x;
}

void doStuff(std::unique_ptr<UPTest> o)
{
	cout << "Doing stuff with UPTest " << o->_id << el;
}
#endif
void uniquePtr()
{
#if CPP_VER > 98
		std::unique_ptr<UPTest> x = getSomething();
		std::unique_ptr<UPTest> y = getAnotherOne();

		//std::unique_ptr<UPTest> x1(x);    // ERROR! error C2280: 'std::unique_ptr<UPTest... : attempting to reference a deleted function
		std::unique_ptr<UPTest> x2;
		//x2 = x;                   // ERROR! error C2280: 'std::unique_ptr<UPTest... : attempting to reference a deleted function

		x2 = std::move(y); //Move semantics required, moves the own pointer from one to the other

		doStuff(std::move(x)); //Pass along the pointer and ownership
		assert(x == nullptr);
		assert(y == nullptr);
		assert(x2 != nullptr);
		cout << x2->_id << el;

		//But you can still do this
		UPTest* pUP = new UPTest(12);

		auto up3 = std::unique_ptr<UPTest>(pUP);
			
		//delete pUP; //BAD, when up3 goes out of scope it will double delete the pUP pointer

#endif
}