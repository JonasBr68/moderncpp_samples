#include "stdafx.h"
#include "EnforceUniquePtr.h"

#include <memory>

#if CPP_VER > 98
class EnforceUniquePtr
{
public:
	static std::unique_ptr<EnforceUniquePtr> create()
	{
		static int i = 1;
		//return std::unique_ptr<EnforceUniquePtr>(new EnforceUniquePtr(i++));
		return std::make_unique<EnforceUniquePtr>(i++);
	}
	int i() const { return m_i; }
private:
	//Allow unique_ptr to call our destructor
	friend std::default_delete<EnforceUniquePtr>; 
	//Allow unique_ptr to call our constructor

	//Uses 'pefect' forwarding, maintains lvalue/rvalue nature of the function/constructor arguments
	friend std::unique_ptr<EnforceUniquePtr> std::make_unique<EnforceUniquePtr>(int&&); 
	

	EnforceUniquePtr(int i):
		m_i(i)
	{
	}
	~EnforceUniquePtr() {
		cout << "destructing EnforceUniquePtr " << m_i << el;
	}
	int m_i { 0 };
};


void doThingsWithRawPointer(const EnforceUniquePtr* p)
{
	cout << "Processing EnforceUniquePtr " << p->i() << el;
}
#endif
void enforceUniquePtr()
{
#if CPP_VER > 98
	auto o = EnforceUniquePtr::create();

	//auto sp = new EnforceUniquePtr(); //error C2248: 'EnforceUniquePtr::EnforceUniquePtr' : cannot access private member declared in class 'EnforceUniquePtr'

	//auto p = std::make_unique<EnforceUniquePtr>(1); //NO, constructor private
	//auto p2 = std::unique_ptr<EnforceUniquePtr>(new EnforceUniquePtr(2)); //NO, constructor private

	//But you can still do this
	EnforceUniquePtr* pEU = o.get();

	doThingsWithRawPointer(pEU); //Typically to call code that already expects a raw pointer

	std::unique_ptr<EnforceUniquePtr> p2 = std::unique_ptr<EnforceUniquePtr>(pEU); //Bad idea, now two owners of pointer
	
	
	pEU = p2.release(); //Releases ownership p2 is now cleared up, and p is only unique_ptr owning it
	
	assert(p2 == nullptr);

	auto o2 = EnforceUniquePtr::create();
	o2 = nullptr; //Will delete any pointer in o and reset it.

	//delete pEU; //Cannot do this, destructor is private
#endif
}
