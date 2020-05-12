#pragma once
#ifndef GRAVITY_LIB_FOUNDATION
#define GRAVITY_LIB_FOUNDATION


#include <stdlib.h>
#include <crtdbg.h>
#include <initializer_list>


namespace Gfl 
{


#define BITS_IN_A_BYTE 8


#ifdef _WIN32

	using uint8		= unsigned __int8;
	using uint16	= unsigned __int16;
	using uint32	= unsigned __int32;
	using uint64	= unsigned __int64;

	using int8		= __int8;
	using int16		= __int16;
	using int32		= __int32;
	using int64		= __int64;

	using byte		= signed char;
	using ubyte		= unsigned char;

	using float32	= float;
	using float64	= double;

#endif

	template <typename T>
	const T& Min(const T& A, const T& B)
	{
		return (A < B) ? A : B;
	}

	template <typename T>
	const T& Max(const T& A, const T& B)
	{
		return (A > B) ? A : B;
	}

	template <typename T>
	const T& Clamp(const T& Val, const T& Lo, const T& Hi)
	{
		return Max(Lo, Min(Val, Hi));
	}

}


#include "GrvtTemplates.h"


#endif // !GRAVITY_LIB_FOUNDATION
