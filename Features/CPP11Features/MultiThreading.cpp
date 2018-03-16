#include "stdafx.h"
#include "MultiThreading.h"
#if CPP_VER > 98
#include <atomic>
#include <mutex>
#endif
#include <memory>


std::once_flag flag;

void do_something() {
	int captured = 4;
	std::call_once(flag, [&captured]() {std::cout << "Called once " << captured << std::endl; });

	std::cout << "Called each time" << std::endl;
}

void callOnce() {

	for (int i = 0; i < 5; i++)
	{
		do_something();
	}

}



#if CPP_VER > 98
//http://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/
class Singleton {
	//Not possible making it a unique_ptr
	//see https://stackoverflow.com/questions/13866743/thread-safe-unique-ptr-move
	//static std::atomic<std::unique_ptr<Singleton>> s_instance;
	static std::atomic<Singleton*> s_instance;
	static std::mutex s_mutex;
	friend class ThreadCleanup;
	static std::atomic<int> s_counter;
public:
	Singleton()
	{
		cout << "Singleton()" << el;
		s_counter++;
		assert(s_counter.load() < 3);
	}
	~Singleton()
	{
		cout << "~Singleton()" << el;
		assert(s_counter > 0);
		s_counter--;
	}
	
	//Avoid copying by accident, delete or remove compiler generated defaults
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	//DCLP - Double Check Locking Pattern now safe in C++ 11
	static Singleton* getInstance() {
		Singleton* tmp = s_instance.load();
		if (tmp == nullptr) {
			std::lock_guard<std::mutex> lock(s_mutex);
			tmp = s_instance.load();
			if (tmp == nullptr) {
				tmp = new Singleton;
				s_instance.store(tmp);
			}
		}
		return tmp;
	}
	//Even better, now with C++ 11 static initializers
	static Singleton& getSingleton() {
		static Singleton instance;
		return instance;
	}

};
std::mutex Singleton::s_mutex;
std::atomic<int> Singleton::s_counter;
std::atomic<Singleton*> Singleton::s_instance = nullptr;


class ThreadCleanup {
public:
	//Only ever called on thread exit cleanup
	~ThreadCleanup() {
		cout << "~ThreadCleanup()" << el;
		Singleton* tmp = Singleton::s_instance.load();
		if (tmp)
		{
			delete tmp;
			Singleton::s_instance.store(nullptr);
		}
	}
};

static std::unique_ptr<ThreadCleanup> clean_up_program = std::make_unique<ThreadCleanup>();

#endif

#if CPP_VER > 110
//thread_local is in C++ 11 but only implemented in VS 2015
thread_local static std::unique_ptr<ThreadCleanup> clean_up_thread = std::make_unique<ThreadCleanup>();
#endif

// C++ 20 built in support http://en.cppreference.com/w/cpp/memory/shared_ptr/atomic2
//class SingletonShared {
//	static std::atomic<std::shared_ptr<SingletonShared>> s_sharedInstance;
//public:
//	~SingletonShared()
//	{
//		cout << "~SingletonShared()" << el;
//	}
//	static std::shared_ptr<SingletonShared> getInstance() {
//		std::shared_ptr<SingletonShared>tmp = s_sharedInstance.load();
//		if (tmp == nullptr) {
//			std::shared_ptr<SingletonShared> potentialSingleton = std::make_shared<SingletonShared>();
//			tmp = s_sharedInstance.exchange(potentialSingleton);
//		}
//		return s_sharedInstance;
//	}
//};
//
//std::atomic<std::shared_ptr<SingletonShared>> SingletonShared::s_sharedInstance;


//Example from https://baptiste-wicht.com/posts/2012/03/cp11-concurrency-tutorial-part-2-protect-shared-data.html
struct Counter {
	int value;

	Counter() : value(0) {}

	void increment() {
		++value;
	}

	void decrement() {
		if (value == 0) {
			throw "Value cannot be less than 0";
		}

		--value;
	}
};

struct ConcurrentSafeCounter {
	std::mutex mutex;
	Counter counter;

	void increment() {
		std::lock_guard<std::mutex> guard(mutex); //Exception safe, scope resolves lifetime
		counter.increment();
	}

	void decrement() {
		std::lock_guard<std::mutex> guard(mutex); //Exception safe, scope resolves lifetime
		counter.decrement();
	}
};

void safeCounter() {
	ConcurrentSafeCounter counter{};

	counter.increment();
	counter.decrement();
}

void atomicSingleton() {
#if CPP_VER > 98
	std::atomic<int> i = 23;
	i += 2;
	cout << i << el;
	
	auto instance = Singleton::getInstance();
	auto instance2 = Singleton::getInstance();
	assert(instance == instance2);

	//below same as auto&
	decltype(Singleton::getSingleton()) instanceStatic = Singleton::getSingleton();
	
	//Dont forget the & it will make a copy if not...
	//deleting copy constructor protects 
	//auto instanceStatic2 = Singleton::getSingleton();
	auto& instanceStatic2 = Singleton::getSingleton();

	// C++ 20 built in support http://en.cppreference.com/w/cpp/memory/shared_ptr/atomic2
	//auto sharedSingleton = SingletonShared::getInstance();
	//auto sharedSingleton2 = SingletonShared::getInstance();
	//assert(sharedSingleton.get() == sharedSingleton2.get());
#endif
}
