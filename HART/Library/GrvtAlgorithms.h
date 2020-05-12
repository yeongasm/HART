#pragma once

#ifndef GRAVITY_LIB_ALGORITHMS
#define GRAVITY_LIB_ALGORITHMS

#include "GrvtArray.h"

namespace Gfl
{

	template <typename T>
	void Swap(T* First, T* Second)
	{
		T Temp = *First;
		*First = *Second;
		*Second = Temp;
	}


	template <typename T>
	void Swap(T& First, T& Second)
	{
		Swap<T>(&First, &Second);
	}

	
	template <typename T>
	T& Min(T* First, T* Second)
	{
		return (*First < *Second) ? *First : *Second;
	}


	template <typename T>
	T& Min(T& First, T& Second)
	{
		return Min(&First, &Second);
	}


	template <typename T>
	T& Max(T* First, T* Second)
	{
		return (*First > * Second) ? *First : *Second;
	}


	template <typename T>
	T& Max(T& First, T& Second)
	{
		return Max(&First, &Second);
	}


	template <typename T>
	T& Clamp(const T& Value, T* First, T* Second)
	{
		return Max(Min(Value, *Second), *First);
	}


	template <typename T>
	T& Clamp(const T& Value, T& First, T& Second)
	{
		return Clamp(Value, &First, &Second);
	}

	namespace Ga
	{
		template <typename T>
		int32 Partition(T* Data, int32 Low, int32 High)
		{
			T Pivot = Data[High];
			int32 Index = Low - 1;

			for (int32 i = Low; i <= High; i++)
			{
				if (Data[i] < Pivot)
				{
					Index++;
					Swap(Data[Index], Data[i]);
				}
			}

			Swap(Data[Index + 1], Data[High]);

			return Index + 1;
		}
	}


	template <typename T>
	void BubbleSort(T* Data, size_t Start, size_t End)
	{
		for (size_t i = Start; i < End - 1; i++)
		{
			for (size_t j = Start; j < End - i - 1; j++)
			{
				if (Data[j + 1] < Data[j])
				{
					Swap(Data[j + 1], Data[j]);
				}
			}
		}
	}


	template <typename ContainerType>
	void BubbleSort(ContainerType& Container)
	{
		BubbleSort(Container.First(), 0, Container.Length());
	}


	template <typename T>
	bool QuickSort(T* Data, int32 Start, int32 End)
	{
		if (Start > End)
		{
			return false;
		}

		int32 Pivot = Ga::Partition(Data, Start, End);

		QuickSort(Data, Start, Pivot - 1);
		QuickSort(Data, Pivot + 1, End);

		return true;
	}


	template <typename ContainerType>
	bool QuickSort(ContainerType& Container)
	{
		return QuickSort(Container.First(), 0, (int32)Container.Length() - 1);
	}
}


#endif // !GRAVITY_LIB_ALGORITHMS