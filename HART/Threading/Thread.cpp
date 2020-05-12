#include "Thread.h"
#include "ThreadPool.h"

namespace hart
{
	
	/*JobObject* WorkerThread::Steal(ThreadPool* Pool)
	{
		JobObject* StolenJob = nullptr;

		WorkerThread* ThreadToStealFrom = Pool->GetRandomThread(this);

		if (!ThreadToStealFrom)
		{
			return nullptr;
		}

		std::deque<JobObject*>& StealQueue = ThreadToStealFrom->JobQueue;

		if (!StealQueue.size())
		{
			// For now we return null, but ideally, we would need to put the thread to sleep.
			return nullptr;
		}

		for (JobObject* ToBeStolenJob : StealQueue)
		{
			if (ToBeStolenJob->Status != JobStatus::Job_Free)
				continue;

			StolenJob = ToBeStolenJob;
			StealQueue.pop_back();
		}

		return StolenJob;
	}*/

	const size_t WorkerThread::TotalJobsInThread() const
	{
		return JobQueue.size();
	}

	void WorkerThread::QueueJobIntoThread(JobObject* Job)
	{
		EnterCriticalSection(&QueueLock);
		JobQueue.push_front(Job);
		LeaveCriticalSection(&QueueLock);
	}


	DWORD WINAPI WorkerThread::ExecuteThread(void* Args)
	{
		// Copy instead of taking a reference.
		WorkerArgs Arguments = *(WorkerArgs*)Args;
		
		ThreadPool*		Pool		= Arguments.PoolPtr;
		WorkerThread*	ThisThread	= Arguments.ThreadPtr;

		ThisThread->Id = GetCurrentThreadId();

		JobObject* CurrentJob = nullptr;

		while (true)
		{
			if (!Pool->_IsRunning)
			{
				break;
			}

			if (ThisThread->JobQueue.size())
			{
				EnterCriticalSection(&ThisThread->QueueLock);

				CurrentJob = ThisThread->JobQueue.back();
				ThisThread->JobQueue.pop_back();

				LeaveCriticalSection(&ThisThread->QueueLock);
			}

			// Should read more about lock-free in GEA before implementing this part.
			if (!CurrentJob)
			{
				// Yield time slice to another thread.
				Sleep(1);
				continue;
			}

			CurrentJob->Status = JobStatus::Job_Taken;
			CurrentJob->Function(CurrentJob->Arguments);
			CurrentJob->Status = JobStatus::Job_Completed;

			delete CurrentJob->Arguments;

			Pool->_NumJobs--;

			delete CurrentJob;
			CurrentJob = nullptr;

			if (!(Pool->_NumJobs != 0))
			{
				SetEvent(Pool->Complete);
			}
		}

		return 0;
	}
}