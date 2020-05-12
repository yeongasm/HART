#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <deque>

#include <Library/GrvtFoundation.h>

#include "JobObject.h"


namespace hart
{

	class ThreadPool;
	class WorkerThread;

	struct WorkerArgs
	{
		WorkerThread*	ThreadPtr	= nullptr;
		ThreadPool*		PoolPtr		= nullptr;
	};

	
	/**
	* Lock free job based thread.
	* "Lock free" is not about not using concurrency locks but rather not to block the thread when there is no job.
	* The term "Blocking free" should be the more appropriate problem.
	*
	*
	*/
	class WorkerThread
	{
	private:
		
		CRITICAL_SECTION		QueueLock;
		std::deque<JobObject*>	JobQueue;
		DWORD					Id;

		friend class ThreadPool;

	public:

		WorkerThread() : 
			JobQueue(), Id(0) {}

		~WorkerThread() {}

		//JobObject*			Steal				(ThreadPool* Pool);
		const size_t		TotalJobsInThread	() const;
		void				QueueJobIntoThread	(JobObject* Job);

		static DWORD WINAPI ExecuteThread		(void* Agrs);
	};

}