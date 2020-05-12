#pragma once

#include <deque>
#include <Library/GrvtFoundation.h>

namespace hart
{

	enum class JobStatus : Gfl::uint8
	{
		Job_Free		= 0x00,
		Job_Taken		= 0x01,
		Job_Completed	= 0x02
	};

	using FuncPtr = void(*)(void*);

	struct JobObject
	{
		FuncPtr		Function; // 8
		void*		Arguments;
		JobStatus	Status; // 1
	};

}