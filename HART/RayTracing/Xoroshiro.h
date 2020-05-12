#pragma once
#include <stdint.h>

/**
* xoroshiro PRNG.
* Source code taken from http://prng.di.unimi.it/
*/
namespace hart
{

	inline uint32_t rotl(const uint32_t x, int k) 
	{
		return (x << k) | (x >> (32 - k));
	}


	namespace Internal
	{
		class SplitMix64
		{
		private:

			uint64_t _Num;

		public:

			SplitMix64(uint64_t Seed) :
				_Num(Seed) {}


			uint64_t operator() ()
			{
				uint64_t Result = (_Num + 0x9e3779b97f4a7c15);
				Result = (Result ^ (Result >> 30)) * 0xbf58476d1ce4e5b9;
				Result = (Result ^ (Result >> 27)) * 0x94d049bb133111eb;

				return Result ^ (Result >> 31);
			}
		};
	}


	class Rng32
	{
	private:

		uint32_t s[4];

	public:

		Rng32(uint64_t Seed)
		{
			Internal::SplitMix64 Seeder(Seed);
			s[0] = (uint32_t)Seeder();
			s[1] = (uint32_t)Seeder();
			s[2] = (uint32_t)Seeder();
			s[3] = (uint32_t)Seeder();
		}

		uint32_t operator() ()
		{
			const uint32_t Result = s[0] + s[3];

			const uint32_t T = s[1] << 9;

			s[2] ^= s[0];
			s[3] ^= s[1];
			s[1] ^= s[2];
			s[0] ^= s[3];

			s[2] ^= T;

			s[3] = rotl(s[3], 11);

			return Result;
		}

		/**
		* This is the jump function for the generator. It can be used to generate 2^64 non-overlapping subsequences for parallel computations.
		*/
		void Jump()
		{
			static const uint32_t JUMP[] = { 0x8764000b, 0xf542d2d3, 0x6fa035c3, 0x77f2db5b };

			uint32_t s0 = 0;
			uint32_t s1 = 0;
			uint32_t s2 = 0;
			uint32_t s3 = 0;
			for (int i = 0; i < sizeof JUMP / sizeof * JUMP; i++)
				for (int b = 0; b < 32; b++) {
					if (JUMP[i] & UINT32_C(1) << b) {
						s0 ^= s[0];
						s1 ^= s[1];
						s2 ^= s[2];
						s3 ^= s[3];
					}
					operator()();
				}

			s[0] = s0;
			s[1] = s1;
			s[2] = s2;
			s[3] = s3;
		}

		uint32_t Min()
		{
			return 0;
		}

		uint32_t Max()
		{
			return ~ uint32_t(0);
		}
	};

	template <typename T>
	T UnitRand(Rng32& PRNG)
	{
		return static_cast<T>(PRNG()) / static_cast<T>(PRNG.Max());
	}
}