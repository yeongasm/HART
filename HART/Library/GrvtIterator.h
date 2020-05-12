#pragma once
#ifndef GRAVITY_LIB_ITERATOR
#define GRAVITY_LIB_ITERATOR


namespace Gfl 
{

	/**
	* Gravity's implementation of a forward iterator.
	*/
	template <typename ElementType>
	class Iterator 
	{
	private:

		ElementType* Pointer;

	public:

		Iterator() : 
			Pointer(nullptr) {}
		
		~Iterator() 
		{ 
			Pointer = nullptr; 
		}

		Iterator(ElementType* Rhs) :
			Pointer(Rhs) {}

		Iterator(const Iterator& Rhs)
		{
			*this = Rhs;
		}

		Iterator(Iterator&& Rhs)
		{
			*this = Move(Rhs);
		}

		Iterator& operator= (const Iterator& Rhs)
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) 
				Pointer = Rhs.Pointer;
			
			return *this;
		}

		Iterator& operator= (Iterator&& Rhs)
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) 
			{
				Pointer = Rhs.Pointer;
				new (&Rhs) Iterator();
			}

			return *this;
		}

		Iterator& operator++ () 
		{
			++Pointer;
			return *this;
		}

		Iterator& operator-- () 
		{
			--Pointer;
			return *this;
		}

		bool operator!= (const Iterator& Rhs) const 
		{
			return Pointer != Rhs.Pointer;
		}

		ElementType& operator* () const 
		{
			return *Pointer;
		}

		Iterator& operator+ (size_t Offset) 
		{
			Pointer += Offset;
			return *this;
		}

		Iterator& operator+ (const Iterator& Rhs) const
		{
			return Pointer + Rhs.Pointer;
		}

		Iterator& operator- (size_t Offset) 
		{
			Pointer -= Offset;
			return *this;
		}

		Iterator& operator- (const Iterator& Rhs) const
		{
			return Pointer - Rhs.Pointer;
		}

		ElementType& operator[] (size_t Index) 
		{
			return *(Pointer + Index);
		}

	};

}


#endif // !GRAVITY_LIB_ITERATOR
