#include "stdafx.h"
#include "SharedPtr.h"
#include <memory>

namespace SharedPtrExamples {

	class Simple {
	public:
		//int id{ 0 };
		int id;
		int initialized;
	};

	class Holder {
	public:
		std::shared_ptr<Holder> ptr1;
		std::weak_ptr<Holder> ptr2;

		~Holder()
		{
			std::shared_ptr<Holder> other = ptr2.lock();
			if (other)
			{
				cout << "other still alive" << el;
			}
		}
	};

}

using namespace SharedPtrExamples;

void sharedPtrRefCounting()
{
	std::shared_ptr<Holder> a = std::make_shared<Holder>();
	std::shared_ptr<Holder> b = std::make_shared<Holder>();
	
	std::shared_ptr<Holder> c = a;
	cout << "a use_count " << a.use_count() << el; // a use_count 2
	
	std::shared_ptr<Holder> d = a;
	cout << "a use_count " << a.use_count() << el; // a use_count 3

	assert(c.use_count() == d.use_count());

	cout << "b use_count " << b.use_count() << el; // b use_count 1

}

void sharedPtrCyclical()
{

	std::shared_ptr<Holder> a = std::make_shared<Holder>();
	std::shared_ptr<Holder> b = std::make_shared<Holder>();

	a->ptr1 = b; // not quite a cycle yet
	a->ptr2 = b;
	b->ptr2 = a; // now we got a cycle but not strong

	a = nullptr;

}
