#include "stdafx.h"
#include "ConditionVariables.h"
#include <condition_variable>
#include <thread>
#include <vector>
#include <atomic>
#include <conio.h> //Windows only


//Simple sample tell a thread to wake up
std::mutex awakeThreadMutex;
std::condition_variable signal_thread;


void waitOnSignal()
{
	cout << "Thread created" << el;
	std::unique_lock<std::mutex> locker(awakeThreadMutex);
	signal_thread.wait(locker); //Note this can wake up without a signal being sent on some implementations
	cout << "Thread maybe signaled, or spurious wake up" << el;
}



void startThreadAndSignal()
{
	std::thread t(waitOnSignal);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	signal_thread.notify_one(); //Or signal_thread.notify_one(); it is the same here

	t.join(); //Wait for thread to terminate, if not undefined/bad things happens


}

std::atomic<bool> threadSignaled = false;

void waitOnSignal2()
{
	cout << "Thread created in waitOnSignal2" << el;
	std::unique_lock<std::mutex> locker(awakeThreadMutex);

	while (!threadSignaled) //Guarantees to protect if a spurious wake up happens
	{
		signal_thread.wait(locker);
	}

	//Alternative using wait() with predicate
	//signal_thread.wait(locker, []() { return threadSignaled.load(); });

	cout << "Thread signaled in waitOnSignal2" << el;
}

void startThreadAndSignal2()
{

	 //Version safe against spurious wake ups
	std::thread t2(waitOnSignal2);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	threadSignaled = true;
	signal_thread.notify_one(); //Or signal_thread.notify_one(); it is the same here, first thread is gone

	t2.join(); //Wait for thread to terminate, if not undefined/bad things happens

}


std::mutex alternateThreadsMutex;
std::condition_variable runThread1condition;
std::atomic<bool> runThread1 = false;
std::atomic<bool> runThread2 = false;
std::condition_variable runThread2condition;
std::vector<int> threadWork;

void thread1()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(alternateThreadsMutex);
		runThread1condition.wait(lock, []() { return runThread1.load(); });
		auto workSize = threadWork.size();
		if (workSize < 1000)
			threadWork.push_back(1);

		runThread1 = false;
		runThread2 = true;
		lock.unlock();
		
		runThread2condition.notify_one();
		if (++workSize >= 1000)
		{
			break;
		}
	}
}

void thread2()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(alternateThreadsMutex);
		runThread2condition.wait(lock, []() { return runThread2.load(); });
		auto workSize = threadWork.size();
		if (workSize < 1000)
			threadWork.push_back(2);

		runThread2 = false;
		runThread1 = true;
		lock.unlock();
		runThread1condition.notify_one();
		if (++workSize >= 1000)
		{
			break;
		}
	}
}


void alternateThreads()
{
	std::thread t1(thread1);
	std::thread t2(thread2);

	runThread2 = true;
	runThread2condition.notify_one();

	t1.join();
	t2.join();

	cout << threadWork.size() << el;
	cout << threadWork[0] << " " << threadWork[1] << " " << threadWork[2] << " " << threadWork[3] << el;
}



//Start and pause threads using condition_variable
class ThreadWorkers
{
public:
	ThreadWorkers(int numWorkers) : m_numWorkers { numWorkers }
	{
		threadCommands = new std::atomic<Command>[m_numWorkers] {};
		for (int i = 0; i < m_numWorkers; i++)
		{
			m_threads.push_back(std::thread(&ThreadWorkers::DoWork, this, i));
		}
	}
	enum class Command
	{
		Wait = 0,
		Run,
		Exit,
		Invalid
	};

	void issueThreadCommand(int threadIndex, Command cmd)
	{
		threadCommands[threadIndex].store(cmd);
		m_threadSignaling.notify_all();
	}
	void commandAllThreads(Command cmd)
	{
		for (int i = 0; i < m_numWorkers; i++)
		{
			threadCommands[i].store(cmd);
		}
		m_threadSignaling.notify_all();
	}
	void waitAll() {
		for (auto& thread : m_threads) {
			thread.join();
		}
	}
private:
	void DoWork(int threadIndex)
	{

		Command lastCommand = Command::Wait;
		while (threadCommands[threadIndex] != Command::Exit)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_threadSignaling.wait(lock, [this, threadIndex, &lastCommand, &lock]() {

				if (lastCommand != threadCommands[threadIndex]) {
					if (threadCommands[threadIndex] == Command::Wait)
						cout << "Thread " << threadIndex << " waiting!" << el;
					else if (threadCommands[threadIndex] == Command::Run)
						cout << "Thread " << threadIndex << " started working!" << el;
				}
				lastCommand = threadCommands[threadIndex];
				return threadCommands[threadIndex] > Command::Wait;
			});

			switch(lastCommand)
			{
			case Command::Run:
				lock.unlock(); //Dont sleep with the lock
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				break;
			case Command::Exit:
				cout << "Thread " << threadIndex << " exiting!" << el;
				break;
			default:
				assert(false); //Something is wrong
			}
		}
		
	}
	std::condition_variable m_threadSignaling;
	std::mutex m_mutex;
	std::vector<std::thread> m_threads;
	std::atomic<Command>* threadCommands {};
	int m_numWorkers{};
};

void stopStartThreads()
{

	ThreadWorkers workers(6);


	cout << "Type sX to start a thread, where X is 0 to 6!:" << el;
	cout << "Type pX to pause a thread!:" << el;
	cout << "Type P to pause all threads!:" << el;
	cout << "Type X to quit!:" << el;
	char cmd = 'r';
	std::string input;


	while (cmd != 'X')
	{
		//Read and echo the input character
		cmd = _getch(); //conio.h windows console, not portable
		cout << cmd << '\r';
		
		switch (cmd)
		{
		case 's':
		{
			int threadId = _getch() - '0';
			if (-1 < threadId && threadId < 7)
			{
				workers.issueThreadCommand(threadId, ThreadWorkers::Command::Run);
			}
			break;
		}
		case 'p':
		{
			int threadId = _getch() - '0';
			if (-1 < threadId && threadId < 7)
			{
				workers.issueThreadCommand(threadId, ThreadWorkers::Command::Wait);
			}
			break;
		}
		case 'P':
		{
			workers.commandAllThreads(ThreadWorkers::Command::Wait);
			break;
		}
		case 'X':
			workers.commandAllThreads(ThreadWorkers::Command::Exit);
			break;
		}
	}
	workers.waitAll();
}




//Set USE_LOCAL_COND_VAR to use 'home cooked' version of condition_variable using win32 Mutex or CriticalSection see WinConditionVar.h
#define USE_LOCAL_COND_VAR 0
#if USE_LOCAL_COND_VAR 
#include "WinConditionVar.h"
namespace NS = win;
#else
namespace NS = std;
#endif


//Original example from https://baptiste-wicht.com/posts/2012/04/c11-concurrency-tutorial-advanced-locking-and-condition-variables.html
struct BoundedBuffer {
	int* buffer;
	int capacity;

	int front;
	int rear;
	int count;

	NS::mutex lock;

	NS::condition_variable not_full;
	NS::condition_variable not_empty;

	BoundedBuffer(int capacity) : capacity(capacity), front(0), rear(0), count(0) {
		buffer = new int[capacity];
	}

	~BoundedBuffer() {
		delete[] buffer;
	}

	void deposit(int data) {
		//std::unique_lock or lock std::lock_guard http://jakascorner.com/blog/2016/02/lock_guard-and-unique_lock.html
		NS::unique_lock<NS::mutex> l(lock);
		assert(l.owns_lock());

		//Unlocks, waits and reaquire lock 
		//Lambda is predicate which returns ​false if the waiting should be continued. 
		//That is wait till notified, if condition not true, keep waiting 
		not_full.wait(l, [this]() {return count != capacity; }); 
		assert(l.owns_lock());
		buffer[rear] = data;
		rear = (rear + 1) % capacity;
		++count;

		//We need to unlock so not_empty can wait on the notification
#if !USE_LOCAL_COND_VAR 
		l.unlock(); //std::unique_lock needed as std::lock_guard cannot be unlocked
		assert(!l.owns_lock());
		not_empty.notify_one();
#else
		not_empty.notify_one();
		l.unlock();
#endif
		
		
	}

	int fetch() {
		NS::unique_lock<NS::mutex> l(lock); //Takes the lock by default, must not call with std::defer_lock
		cout << "fetch owns_lock = " << l.owns_lock() << el;
		assert(l.owns_lock());

		not_empty.wait(l, [this, &l]() { //Unlocks while waiting after condition is false
			assert(l.owns_lock());
			return count != 0; 
		});

		//Alternative, easier to read and understand. Any other differences?
		//while (count == 0)
		//{
		//	not_empty.wait(l); //When signaled, takes lock and wakes up
		//	cout << "fetch woke up" << el;
		//}

		assert(l.owns_lock());
		int result = buffer[front];
		front = (front + 1) % capacity;
		--count;

#if !USE_LOCAL_COND_VAR 
		l.unlock();
		assert(!l.owns_lock());
		not_full.notify_one();
#else
		not_full.notify_one();
		l.unlock();
		assert(!l.owns_lock());
#endif
		return result;
	}
};

void consumer(int id, BoundedBuffer& buffer) {
	for (int i = 0; i < 50; ++i) {
		int value = buffer.fetch();
		std::cout << "Consumer " << id << " fetched " << value << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
}

void producer(int id, BoundedBuffer& buffer) {
	for (int i = 0; i < 75; ++i) {
		buffer.deposit(i);
		std::cout << "Produced " << id << " produced " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}




void conditionVariables() {
	BoundedBuffer buffer(200);

	std::thread c1(consumer, 0, std::ref(buffer));
	std::thread c2(consumer, 1, std::ref(buffer));
	std::thread c3(consumer, 2, std::ref(buffer));
	std::thread p1(producer, 0, std::ref(buffer));
	std::thread p2(producer, 1, std::ref(buffer));


	//Wait for threads to finish, otherwise buffer is gone after this function exists... and undefined behaviour
	c1.join();
	c2.join();
	c3.join();
	p1.join();
	p2.join();

}



