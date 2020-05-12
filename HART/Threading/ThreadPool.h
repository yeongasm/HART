#pragma once

#include <deque>
#include <atomic>
#include <Library/GrvtArray.h>

#include "Thread.h"

namespace hart
{

	class ThreadPool
	{
	private:

		Gfl::Array<HANDLE>			_ThreadHandles;
		Gfl::Array<WorkerThread>	_Workers;
		Gfl::Array<WorkerArgs>		_WorkerArguments;

		Gfl::uint32					_RoundRobinIdx;

	public:

		HANDLE Complete;

		std::atomic<int> _NumJobs;

		bool	_IsRunning;

		ThreadPool() :
			_ThreadHandles(), _Workers(), _WorkerArguments(), _RoundRobinIdx(0), 
			_NumJobs(0), _IsRunning(true) {}

		~ThreadPool() {}

		void			Initialise				();
		void			Shutdown				();
		WorkerThread*	GetRandomThread			(const WorkerThread* CallingThread);

		void			PushJobIntoThread		(FuncPtr Function, void* Argument);

	};

}