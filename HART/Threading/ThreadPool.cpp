#include "ThreadPool.h"

#include <Library/GrvtPair.h>

namespace hart
{

#define HART_THREAD_DEBUG 0

	void ThreadPool::Initialise()
	{
#if HART_THREAD_DEBUG

		size_t TotalThreads = 1;

#else

		SYSTEM_INFO SysInfo;
		GetSystemInfo(&SysInfo);
		size_t TotalCores = SysInfo.dwNumberOfProcessors;

		// Minus 1 (which is the main thread).
		size_t TotalThreads = TotalCores - 1;

#endif

		_ThreadHandles.Reserve(TotalThreads);
		_Workers.Reserve(TotalThreads);
		_WorkerArguments.Reserve(TotalThreads);

		for (size_t i = 0; i < TotalThreads; i++)
		{
			_WorkerArguments[i].PoolPtr	 = this;
			_WorkerArguments[i].ThreadPtr = &_Workers[i];

			_ThreadHandles[i] = CreateThread(NULL, 0, WorkerThread::ExecuteThread, &_WorkerArguments[i], 0, NULL);

			// Based on David Hefferman's answer in https://stackoverflow.com/questions/5919699/proper-usage-of-setthreadaffinitymask,
			// this does not show to produce performance benefits.
			SetThreadAffinityMask(_ThreadHandles[i], i);

			_ASSERT(_ThreadHandles[i] != nullptr);

			InitializeCriticalSection(&_Workers[i].QueueLock);
		}

		Complete = CreateEvent(NULL, FALSE, FALSE, NULL);

		_ASSERT(Complete != NULL);
	}


	void ThreadPool::Shutdown()
	{
		WaitForSingleObject(Complete, INFINITE);
		CloseHandle(Complete);

		_IsRunning = false;

		WaitForMultipleObjects((DWORD)_Workers.Length(), _ThreadHandles.First(), TRUE, INFINITE);

		for (size_t i = 0; i < _ThreadHandles.Length(); i++)
		{
			DeleteCriticalSection(&_Workers[i].QueueLock);
			CloseHandle(_ThreadHandles[i]);
		}

		_ThreadHandles.Release();
		_Workers.Release();
		_WorkerArguments.Release();
	}

	
	WorkerThread* ThreadPool::GetRandomThread(const WorkerThread* CallingThread)
	{
		size_t RandomIndex = 0 + rand() % _Workers.Length() - 1;

		WorkerThread* RandomThread = &_Workers[RandomIndex];
		
		if (RandomThread == CallingThread)
		{
			return nullptr;
		}

		return RandomThread;
	}

	void ThreadPool::PushJobIntoThread(FuncPtr Function, void* Argument)
	{
		size_t TotalWorkerThreads = _Workers.Length();
		
		if (_RoundRobinIdx == TotalWorkerThreads)
		{
			_RoundRobinIdx = 0;
		}

		JobObject* Job = new JobObject();

		Job->Function = Function;
		Job->Arguments = Argument;
		Job->Status = JobStatus::Job_Free;

		_Workers[_RoundRobinIdx++].QueueJobIntoThread(Job);
		_NumJobs++;
	}

}