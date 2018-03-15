#include "stdafx.h"
#include "PlacementNew.h"

#if CPP_VER > 97
class Label
{
	std::string m_label;
public:

	Label(std::string label) :m_label(label)
	{

	}
	const std::string& getLabel() const {
		return m_label;
	}
};



static char preAllocatedMemory[sizeof(Label) * 5];
#endif


//Not C++ 11 new exactly, good discussion here https://stackoverflow.com/questions/222557/what-uses-are-there-for-placement-new
//'HACK' can be useful - not thread safe
//https://stackoverflow.com/questions/15797825/can-i-use-placement-new-to-reset-an-object-within-a-shared-ptr
void replaceContentOfPointer()
{
	auto L1 = new Label("swapWithPlacment1");
	
	auto tmp = L1; //Some one has a pointer, or shared_ptr pointing to L1 we need to replace it without giving out new pointer adress

	cout << tmp->getLabel() << el;

	L1->~Label(); //Calling destructor explictly of existing object , if no dynaic allocations in constructor not needed

	new (L1) Label("swapWithPlacment2");

	cout << tmp->getLabel() << el;
}


void placementNew() {

	replaceContentOfPointer();

}