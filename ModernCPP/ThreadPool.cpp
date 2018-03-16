#include "stdafx.h"
#include "ThreadPool.h"
#include <vector>
#include <thread>


static std::vector<std::function<void()>> _tasks;
static std::vector<std::thread> _threads;

class ThreadPool;

static ThreadPool* pPool = nullptr;

class ThreadPool
{
public:
	ThreadPool(int numThreads):m_numThreads(numThreads){
		//Initialize and create numThreads and store them in _threads
	}
	void addTask(std::function<void()> task) {
		//Add task to _tasks

	}
	void waitAll() {
		//wait for _tasks to be empty and all threads idle
	}
	~ThreadPool(){}
private:
	int m_numThreads{};
};



void simpleThreadPool()
{

	//Initialize pPool once, and replace raw pPool pointer with a std::unique_ptr;

	pPool->addTask([]() -> void { cout << "hello" << el; });

	pPool->addTask([i = 4]() -> void {
		cout << "hello " << i << el; 
	});

	pPool->waitAll();
}