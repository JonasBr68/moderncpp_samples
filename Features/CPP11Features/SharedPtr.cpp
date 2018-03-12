#include "stdafx.h"
#include "SharedPtr.h"
#include <memory>

class Simple {

};

class Holder {
public:
	std::shared_ptr<Holder> ptr;
};


void sharedPtrCyclical()
{

	std::shared_ptr<Holder> a = std::make_shared<Holder>();
	std::shared_ptr<Holder> b = std::make_shared<Holder>();

	a->ptr = b; // not quite a cycle yet
	b->ptr = a; // now we got a cycle a keeps b alive and b keeps a alive

}
