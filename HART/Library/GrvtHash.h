#pragma once
#ifndef GRAVITY_LIB_HASH
#define GRAVITY_LIB_HASH


#include "GrvtFoundation.h"


namespace Gfl
{
	
	/**
	* Gravity's implementation of the MurmurHash3_x86_32 algorithm.
	*/
	void MurmurHash32(const void* Key, uint32 Len, void* Out, uint32 Seed = 0);


	/**
	* Gravity's default Hash functor.
	* Supplied type must have an overridden First() and Length() function.
	*/
	template <typename Type>
	struct MurmurHash
	{

		uint32 operator() (const Type& Source) const
		{
			uint32 Hash = 0;
			MurmurHash32(Source.First(), (uint32)Source.Length(), &Hash);

			return Hash;
		}

	};

	
	template <>
	struct MurmurHash<int32>
	{
		uint32 operator() (const int32 Source) const
		{
			uint32 Hash = 0;
			MurmurHash32(&Source, sizeof(int32), &Hash);

			return Hash;
		}
	};


	template <>
	struct MurmurHash<uint32>
	{
		uint32 operator() (const uint32 Source) const
		{
			uint32 Hash = 0;
			MurmurHash32(&Source, sizeof(uint32), &Hash);

			return Hash;
		}
	};


	template <>
	struct MurmurHash<float32>
	{
		uint32 operator() (const float32 Source) const
		{
			uint32 Hash = 0;
			MurmurHash32(&Source, sizeof(float32), &Hash);

			return Hash;
		}
	};


	template <>
	struct MurmurHash<float64>
	{
		uint32 operator() (const float64 Source) const
		{
			uint32 Hash = 0;
			MurmurHash32(&Source, sizeof(float64), &Hash);

			return Hash;
		}
	};
}


#endif // !GRAVITY_LIB_HASH
