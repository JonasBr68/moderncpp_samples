#include "stdafx.h"
#include "ThreadPool.h"
#include <vector>
#include <thread>


static std::vector<std::function<void()>> _tasks;
static std::vector<std::thread> _threads;

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
	ThreadPool pool(8); //Init with max threads

	pool.addTask([]() -> void { cout << "hello" << el; });

	pool.addTask([i = 4]() -> void { 
		cout << "hello " << i << el; 
	});

	pool.waitAll();
}