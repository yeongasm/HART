#pragma once
#include <stdlib.h>
#include <Library/GrvtTemplates.h>
#include <Library/GrvtFoundation.h>

namespace hart
{

#define BYTE(n)		n * sizeof(char)
#define KILOBYTE(n) n * BYTE(1024) 
#define MEGABYTE(n) n * KILOBYTE(1024)
#define GIGABYTE(n) n * MEGABYTE(1024)


	class StackAllocator;

	template <typename T>
	class Pointer
	{
	private:

		StackAllocator* AllocatorRef;
		size_t			Offset;

		friend class StackAllocator;

	public:

		Pointer() :
			AllocatorRef(nullptr), Offset(0) {}

		~Pointer()
		{
			AllocatorRef = nullptr;
			Offset = 0;
		}

		Pointer(const Pointer& Rhs)
		{
			*this = Rhs;
		}

		Pointer& operator= (const Pointer& Rhs)
		{
			AllocatorRef = Rhs.AllocatorRef;
			Offset = Rhs.Offset;

			return *this;
		}

		Pointer(Pointer&& Rhs)
		{
			*this = Gfl::Move(Rhs);
		}

		Pointer& operator= (Pointer&& Rhs)
		{
			if (this != &Rhs)
			{
				AllocatorRef = Gfl::Move(Rhs.AllocatorRef);
				Offset = Gfl::Move(Rhs.Offset);

				new (&Rhs) Pointer();
			}

			return *this;
		}

		T& operator* ()
		{
			if (Offset >= AllocatorRef->Offset)
			{
				_ASSERT_EXPR(false, "Attempting to access region of uninitialized memory!");
			}

			return *(T*)(AllocatorRef->StackPtr + Offset);
		}
	};

	
	class StackAllocator
	{
	private:

		using byte = unsigned char;

		byte*	StackPtr;
		size_t	Offset;
		size_t	Capacity;

		bool Realloc(size_t Size)
		{
			byte* TempStack = (byte*)realloc(StackPtr, Size);

			if (!TempStack)
			{
				return false;
			}

			StackPtr = TempStack;
			TempStack = nullptr;

			Capacity = Size;

			return true;
		}

		void Free()
		{
			free(StackPtr);
			StackPtr = nullptr;
		}

		template <typename T>
		friend class Pointer;

	public:

		StackAllocator() :
			StackPtr(nullptr), Offset(0), Capacity(0) {}

		~StackAllocator()
		{
			Free();
		}

		void Initialize(size_t InitialCapacity)
		{
			Realloc(InitialCapacity);
		}


		template <typename T>
		Pointer<T> Allocate()
		{
			if (Offset >= Capacity)
				_ASSERTE(false);

			size_t DataSize = sizeof(T);

			Pointer<T> TempPtr;
			TempPtr.AllocatorRef = this;
			TempPtr.Offset = Offset;

			Offset += DataSize;

			return TempPtr;
		}

	};

}