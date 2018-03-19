#pragma once


//#include <yvals.h>
#include <windows.h>
#include <stdio.h>
#include <atomic>

#define USE_WINMUTEX 1

#ifdef USE_WINMUTEX 
#ifdef USE_WINCRITICAL_SECTION
#undef USE_WINCRITICAL_SECTION
#endif
#else
#ifndef USE_WINCRITICAL_SECTION
#define USE_WINCRITICAL_SECTION 1
#endif
#endif

namespace win
{

	static std::thread::id ZERO_THREAD = std::thread::id();

	class mutexMutex
	{
	public:
		mutexMutex() {
			_hWinMutex = CreateMutex(
				NULL,              // default security attributes
				FALSE,             // initially not owned
				NULL);             // unnamed mutex

		}
		~mutexMutex()
		{
			if (_hWinMutex)
				CloseHandle(_hWinMutex);
		}
		void unlock()
		{
			if (_hWinMutex && owns_lock())
			{
				_threadOwner = ZERO_THREAD;
				if (!ReleaseMutex(_hWinMutex))
				{
					// Handle error.
				}

			}
		}
		void lock()
		{
			if (_hWinMutex)
			{
				if (!owns_lock())
				{
					DWORD dwWaitResult = WaitForSingleObject(
						_hWinMutex,    // handle to mutex
						INFINITE);  // no time-out interval

					switch (dwWaitResult)
					{
						// The thread got ownership of the mutex			
					case WAIT_OBJECT_0:
					case WAIT_ABANDONED: //Owning thread abandon mutex on exit without releasing it, we have the lock now
					{
						assert(_threadOwner == ZERO_THREAD);
						_threadOwner = std::this_thread::get_id();
						break;
					}
					default:
						//Can this happen?
						throw new std::exception("Unexpected result trying to aquire Mutex");
					}
				}
				else
				{
					assert(_threadOwner == std::this_thread::get_id());
				}
			}
			else
			{
				throw new std::exception("No native mutex object");
			}
		}
		bool owns_lock()
		{
			std::thread::id currentOwner = ZERO_THREAD;
			bool isOwner = false;
			if (_hWinMutex)
			{

				DWORD dwWaitResult = WaitForSingleObject(
					_hWinMutex,    // handle to mutex
					0);  // no time-out interval

				switch (dwWaitResult)
				{
					// The thread got ownership of the mutex
				case WAIT_OBJECT_0:
				{

					currentOwner = _threadOwner.load();
					//We are the owner, or no one had it before
					assert(currentOwner == std::this_thread::get_id() || currentOwner == ZERO_THREAD);
					if (currentOwner == ZERO_THREAD)
					{
						//no previous owner, not even us
						isOwner = false;
					}
					else
					{
						assert(_threadOwner == std::this_thread::get_id());
						//We already owned it, do corresponding release
						isOwner = true;
					}
					ReleaseMutex(_hWinMutex);
					return isOwner;
				}
				case WAIT_ABANDONED:
				{
					_threadOwner = ZERO_THREAD;
					ReleaseMutex(_hWinMutex);
					break;
				}
				case WAIT_TIMEOUT: //Mutex has owner as it is non-signaled
				default:
				{
				}
				}
			}
			return false;
		}
	private:
		HANDLE _hWinMutex;
		std::atomic<std::thread::id> _threadOwner{};
	};

	class mutexCriticalSection
	{
	public:
		mutexCriticalSection() {
			InitializeCriticalSectionAndSpinCount(_pWinCS.get(), 0x00000400);

		}
		~mutexCriticalSection()
		{
			if (_pWinCS)
				DeleteCriticalSection(_pWinCS.get());
		}
		void unlock()
		{
			if (_pWinCS && owns_lock())
			{
				_threadOwner = ZERO_THREAD;
				LeaveCriticalSection(_pWinCS.get());
			}
		}
		void lock()
		{
			if (_pWinCS)
			{
				if (!owns_lock())
				{
					EnterCriticalSection(_pWinCS.get());
					_threadOwner = std::this_thread::get_id();
				}
				else
				{
					assert(_threadOwner == std::this_thread::get_id());
				}
			}
			else
			{
				throw new std::exception("No native mutex object");
			}
		}
		bool owns_lock()
		{
			std::thread::id currentOwner = ZERO_THREAD;
			bool isOwner = false;
			if (_pWinCS)
			{
				bool isOwner = TryEnterCriticalSection(_pWinCS.get());
				if (isOwner)
				{
					currentOwner = _threadOwner.load();
					//We are the owner, or no one had it before
					assert(currentOwner == std::this_thread::get_id() || currentOwner == ZERO_THREAD);
					if (currentOwner == ZERO_THREAD)
					{
						//no previous owner, not even us
						isOwner = false;
					}
					else
					{
						assert(_threadOwner == std::this_thread::get_id());
						//We already owned it
						isOwner = true;
					}
					LeaveCriticalSection(_pWinCS.get());
				}
				return isOwner;
			}
			return false;
		}
	private:
		std::unique_ptr<CRITICAL_SECTION> _pWinCS = std::make_unique<CRITICAL_SECTION>();
		std::atomic<std::thread::id> _threadOwner{};
	};

#ifdef USE_WINMUTEX 
	using mutex = mutexMutex;
#else
	using mutex = mutexCriticalSection;
#endif
	template <class T>
	class unique_lock {
	public:
		unique_lock(mutex& mutex)
		{
			mutex.lock();
			_Mutex = &mutex; //Only store after we got the lock
		}
		bool operator==(const unique_lock<T> &other) const {
			return _Mutex == other._Mutex;
		}
		bool operator!=(const unique_lock<T> &other) const {
			return _Mutex != other._Mutex;
		}
		void unlock()
		{
			if (_Mutex)
			{
				_Mutex->unlock();
			}
		}
		void lock()
		{
			if (_Mutex)
			{
				_Mutex->lock();
			}
		}
		bool owns_lock()
		{
			if (_Mutex)
			{
				return _Mutex->owns_lock();
			}
			return false;
		}
	private:
		mutex * _Mutex{};
	};



	class condition_variable
	{	// class for waiting for conditions
	public:

		condition_variable()
		{	// construct

		}

		~condition_variable() _NOEXCEPT
		{	// destroy
		}

		condition_variable(const condition_variable&) = delete;
		condition_variable& operator=(const condition_variable&) = delete;

		void notify_one() _NOEXCEPT
		{
			// wake up one waiter
			//get the waiting mutex
			win::unique_lock<mutex>* tmp = _Lck.load();
			if (tmp != nullptr)
			{
				assert(tmp->owns_lock());
				tmp->unlock();
			}
			else
			{
				//No one is waiting a no-op
			}
		}

		void notify_all() _NOEXCEPT
		{
			// wake up all waiters
			notify_one();
		}

		void wait(win::unique_lock<mutex>& lock)
		{
			//Entering wait we should have the lock
			assert(lock.owns_lock());
			//set _Lck to current mutex being waited on

			win::unique_lock<mutex>* tmp = nullptr;
			if (!_Lck.compare_exchange_strong(tmp, &lock))
			{
				//someone is already waiting, no problem as long as it on the same mutex
				if (*tmp != lock)
					throw std::exception("Cannot wait on same condition_variable using a different mutext instance");
			}
			//_Lck now contains the lock

			//Relinguish the lock - no locking while waiting
			lock.unlock();

			// wait for signal
			lock.lock();

			_Lck = nullptr;

			//Comming out of wait we always have the lock
			assert(lock.owns_lock());

			// Nothing to do to comply with LWG 2135 because std::mutex lock/unlock are nothrow

		}

		template<class _Predicate>
		void wait(unique_lock<mutex>& _Lck, _Predicate _Pred)
		{	// wait for signal and test predicate
			while (!_Pred())
				wait(_Lck);
		}


	private:
		std::atomic<win::unique_lock<mutex>*> _Lck{};
	};

}
