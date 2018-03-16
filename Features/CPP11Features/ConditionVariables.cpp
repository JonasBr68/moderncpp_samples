#include "stdafx.h"
#include "ConditionVariables.h"
#include <condition_variable>

//Example from https://baptiste-wicht.com/posts/2012/04/c11-concurrency-tutorial-advanced-locking-and-condition-variables.html


struct BoundedBuffer {
	int* buffer;
	int capacity;

	int front;
	int rear;
	int count;

	std::mutex lock;

	std::condition_variable not_full;
	std::condition_variable not_empty;

	BoundedBuffer(int capacity) : capacity(capacity), front(0), rear(0), count(0) {
		buffer = new int[capacity];
	}

	~BoundedBuffer() {
		delete[] buffer;
	}

	void deposit(int data) {
		//std::unique_lock or lock std::lock_guard http://jakascorner.com/blog/2016/02/lock_guard-and-unique_lock.html
		std::unique_lock<std::mutex> l(lock);


		//Unlocks, waits and reaquire lock 
		//Lambda is predicate which returns ​false if the waiting should be continued. 
		//That is wait till notified, if condition not true, keep waiting 
		not_full.wait(l, [this]() {return count != capacity; }); 

		buffer[rear] = data;
		rear = (rear + 1) % capacity;
		++count;

		//We need to unlock so not_empty can wait on the notification
		l.unlock(); //std::unique_lock needed as std::lock_guard cannot be unlocked
		not_empty.notify_one();
	}

	int fetch() {
		std::unique_lock<std::mutex> l(lock);

		not_empty.wait(l, [this]() {return count != 0; });

		int result = buffer[front];
		front = (front + 1) % capacity;
		--count;

		l.unlock();
		not_full.notify_one();

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