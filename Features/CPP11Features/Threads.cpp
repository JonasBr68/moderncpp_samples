#include "stdafx.h"
#include "Threads.h"
#include <thread>
#include <vector>



void hello() {
	std::cout << "Hello from thread " << std::endl;
}

void simpleHello() {
	std::thread t1(hello);
	t1.join();
}

void hello2() {
	std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
}

void multiHello() {
	std::vector<std::thread> threads;

	for (int i = 0; i < 5; ++i) {
		threads.push_back(std::thread(hello2));
	}

	for (auto& thread : threads) {
		thread.join();
	}

}

void  threadWithLambda() {
	std::vector<std::thread> threads;

	for (int i = 0; i < 5; ++i) {
		threads.push_back(std::thread([]() {
			std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
		}));
	}

	for (auto& thread : threads) {
		thread.join();
	}

}

void threads() {

	simpleHello();

	multiHello();

	threadWithLambda();
}