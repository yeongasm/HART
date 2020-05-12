#pragma once
#ifndef GRAVITY_LIB_ARRAY
#define GRAVITY_LIB_ARRAY


#include "GrvtFoundation.h"
#include "GrvtIterator.h"


namespace Gfl 
{

	/**
	* Templated hybrid Array. Can be used as a static Array or dynamic Array.
	*/
	template <class ElementType, size_t Slack = 5>
	class Array 
	{
	public:

		using Iterator = Iterator<ElementType>;

	protected:

		ElementType* Data;
		size_t	Capacity;
		size_t	Len;

	private:

		void Destruct(size_t FromIdx = 0, size_t ToIdx = Len, const bool Reconstruct = false) 
		{
			for (size_t i = FromIdx; i < ToIdx; i++) 
			{
				Data[i].~ElementType();
				if (Reconstruct)
					new (Data + i) ElementType();
			}
		}

		/*
		* Set the length of the Array. Meant for the use of static Arrays.
		* When using the Array dynamically, call Push() instead as the Array knows how to manage it's own space.
		*/
		bool Alloc(size_t Length = 0, bool Append = false)
		{
			ElementType* TempData = nullptr;

			if (Length) 
			{
				// If the function was initiated with a length.
				if (Length < Capacity)
					Destruct(Length, Capacity);

				Capacity = (Append) ? Capacity + Length : Length;
				TempData = (ElementType*)realloc(Data, sizeof(ElementType) * Capacity);

				if (!TempData)
					return false;

				for (size_t i = Len; i < Capacity; i++)
					new (TempData + i) ElementType();
			} 
			else if (!Len || Len == Capacity)
			{
				Capacity = (!Len) ? 1 << Slack : Capacity + (1 << Slack);
				TempData = (ElementType*)realloc(Data, sizeof(ElementType) * Capacity);

				if (!TempData)
					return false;

				for (size_t i = Len; i < Capacity; i++)
					new (TempData + i) ElementType();
			}

			Data = TempData;

			return true;
		}

		/**
		* We now use perfect forwarding to distinguish if the pushed in element a lvalue or rvalue.
		* This solves the problem of custom data types deleting copy assignment operator or move assignment operator.
		*/
		template <class... ForwardType>
		void StoreObject(ForwardType&&... Element)
		{
			if (Len == Capacity)
			{
				Alloc();
			}

			Data[Len] = ElementType(Forward<ForwardType>(Element)...);
		}

	public:

		/**
		* Default constructor used to initialise the empty Array.
		*/
		Array() : 
			Data(nullptr), Capacity(0), Len(0) {}


		~Array() 
		{
			Release();
		}


		/*
		* Constructor to initialise a 'static' Array of sorts.
		* The ability to Push() new elements into the Array is still do-able.
		*/
		Array(size_t Length) : Data(nullptr), Capacity(0), Len(0)
		{
			Alloc(Length);
		}


		/**
		* Copy constructor used to copy elements from another Array.
		*/
		Array(const Array& Rhs) 
		{
			*this = Rhs;
		}


		/**
		* Move semantics.
		*/
		Array(Array&& Rhs)
		{
			*this = Move(Rhs);
		}


		/**
		* Constructor to initialise the Array with an initialiser list.
		*/
		Array(const std::initializer_list<ElementType>& InitList) : Array()
		{
			//if (InitList.size() > Len)
			//{
			//	if (!Alloc((size_t)InitList.size()))
			//	{
			//		// If upsizing the Array fails, throw an assertion.
			//		_ASSERTE(false);
			//	}
			//}

			//Len = (size_t)InitList.size();
			Append(InitList);
		}


		/**
		* Operator to copy the contents from another Array into this one.
		*/
		Array& operator= (const Array& Rhs)
		{
			if (this != &Rhs)
			{
				if (Capacity < Rhs.Capacity)
				{
					Alloc(Rhs.Capacity);
				}

				//Capacity = Rhs.Capacity;
				Len = Rhs.Len;
				//memcpy(pArr, Other.pArr, Other.cusPos - 1);
				for (size_t i = 0; i < Rhs.Len; i++)
				{
					Data[i] = Rhs.Data[i];
				}
			}

			return *this;
		}

		/**
		* Operator for move semantics.
		*/
		Array& operator= (Array&& Rhs)
		{
			// Gravity will throw an assertion if you are trying to perform a move semantic on itself in debug.
			_ASSERTE(this != &Rhs);

			if (this != &Rhs)
			{
				Data = Rhs.Data;
				Capacity = Rhs.Capacity;
				Len = Rhs.Len;

				new (&Rhs) Array();
			}

			return *this;
		}

		ElementType& operator[] (size_t Index)
		{
			// The index specified exceeded the internal buffer size of the Array.
			//_ASSERTE(Index < Len);

			// In a scenario where the [] operator is used to assign values to the Array, the Array needs to increase it's internal capacity when needed.
			if (Index >= Capacity)
				Alloc(Index);

			// Keeps track of the last element index inside the Array.
			if (Len <= Index)
				Len = Index + 1;

			return Data[Index];
		}

		const ElementType& operator[] (size_t Index) const 
		{
			// The index specified exceeded the internal buffer size of the Array.
			_ASSERTE(Index < Len);
			return Data[Index];
		}

		/**
		* Allocate the specified amount of space for the Array increasing the internal capacity.
		*
		* @param Length - The desired newly allocated internal buffer size.
		* @param Append - Appends to specified allocated size to the current internal capacity.
		*/
		bool Reserve(size_t Length, bool Append = true)
		{
			return Alloc(Length, Append);
		}

		/**
		* Free all the elements in the Array, releasing it's contents.
		*/
		void Release() 
		{
			Destruct(0, Capacity);
			free(Data);

			new (this) Array();
		}

		/**
		* Push an element into the the back of the Array.
		* If elements were assigned via the static method and there are empty slots within the Array, the Array class would
		* ignore the index of the gap and push new elements to the back of the Array.
		* First-In-Last-Out implementation.
		* Returns the index of the newly allocated Element within the Array.
		*
		* @param Element - An element to be pushed into the back of the Array.
		*/
		size_t Push(const ElementType& Element)
		{
			StoreObject(Element);
			return Len++;
		}

		/**
		* Push an element into the the back of the Array.
		* If elements were assigned via the static method and there are empty slots within the Array, the Array class would
		* ignore the index of the gap and push new elements to the back of the Array.
		* First-In-Last-Out implementation.
		* Returns the index of the newly allocated Element within the Array.
		*
		* @param Element - An element to be pushed into the back of the Array.
		*/
		size_t Push(ElementType&& Element)
		{
			StoreObject(Move(Element));
			return Len++;
		}

		/**
		* Push an element into the the back of the Array.
		* If elements were assigned via the static method and there are empty slots within the Array, the Array class would
		* ignore the index of the gap and push new elements to the back of the Array.
		* First-In-Last-Out implementation.
		* Returns a reference / pointer to the newly allocated Element within the Array.
		*
		* @param Element - An element to be pushed into the back of the Array.
		*/
		template <class Type>
		decltype(auto) Insert(Type&& Element) 
		{
			size_t Index = Push(Forward<Type>(Element));
			return Data[Index];
		}

		/**
		* Appends an existing Array and copy or move it's elements into the back of the Array.
		* Destroys contents from the copied Array and constructs a new one by default.
		* Returns the index of the last element after appending.
		*
		* @param Other   - The foreign Array that is being appended.
		* @param Release - Release the foreign Array's memory. [Default] true.
		*/
		size_t Append(const Array& Rhs, bool Release = true)
		{
			MoveOrCopy(Rhs, false, Release);
			return Len;
		}

		/**
		* Appends new elements into the Array with an initializer list.
		* Returns the index of the last element after appending.
		*
		* @param InitList - A foreign initializer list.
		*/
		size_t Append(const std::initializer_list<ElementType>& InitList)
		{
			for (size_t i = 0; i < InitList.size(); i++)
				Push(*(InitList.begin() + i));

			return Len - 1;
		}

		/**
		* Append mutiple elements into the Array by supplying a beginning pointer and the total elements from the foreign container.
		* If the amount inserted exceeds the the internal buffer of the Array, the Array memory reallocation is done.
		* Does not destroy the foreign container's contents.
		* Returns the index of the last element within the Array after appending.
		*
		* @param Begin	- Foreign container's beginning pointer.
		* @param Length	- Foreign container's end pointer.
		*/
		size_t Append(Iterator Begin, size_t Length)
		{
			for (size_t i = 0; i < Length; i++)
				Push(Begin[i]);

			return Len - 1;
		}

		/**
		* Moves all the element from the other Array into the existing one or simply copies from it.
		* Performs a move by default.
		*
		* @param Other      - The foreign Array that is being moved/copied.
		* @param RemLocal   - Clears all the existing elements in the local Array. [Default] true.
		* @param RemForeign - Clears all the existing element in the foreign Array. [Default] true.
		*/
		void MoveOrCopy(Array& Rhs, bool RemoveLocal = true, bool RemoveForeign = true)
		{
			if (RemoveLocal)
				Release();

			for (size_t i = 0; i < Rhs.Len; i++)
				Push(Rhs[i]);

			if (RemoveForeign)
				Rhs.Release();
		}

		/**
		* Pop an element from the back of the Array.
		* Last-In-First-Out implementation.
		* Returns the last index in the element.
		*
		* @param Count	  - The amount of element(s) to be removed. [Default] 1.
		* @param Shrink	  - Reduce the size of the internal buffer allocator for the Array. [Default] false.
		*/
		size_t Pop(size_t Count = 1, bool Shrink = false)
		{
			_ASSERTE(Len >= 0);

			Destruct(Len - Count, Len, true);
			Len -= Count;
			Len = (Len < 0) ? 0 : Len;

			if (Shrink)
				ShrinkToFit();

			return Len;
		}

		/**
		* Pops an element at the specified index.
		* Optionally moves the contents of the Array. Optionally shrinks the length of the Array.
		* Reconstructs a fresh state in that index by calling the object's constructor.
		*
		* @param Idx	- The variable's index within the container.
		* @param Move	- Moves the contents within the container closing any gaps in indices. [Default] true.
		* @param Shrink	- Reduces the internal buffer size of the container. [Default] false.
		*/
		void PopAt(size_t Index, bool Move = true, bool Shrink = false)
		{
			// The element should reside in the container.
			_ASSERTE(Index < Len);

			Data[Index].~ElementType();

			if (Move) {
				memmove(Data + Index, Data + Index + 1, sizeof(ElementType) * (--Len - Index));
				new (Data + Len) ElementType();
			} else {
				new (Data + Index) ElementType();
			}

			if (Shrink)
				ShrinkToFit();
		}

		/**
		* Pops all elements out of the container.
		* Unlike Release(), Empty() maintains the internal capacity of the Array.
		* Pushing a new element would insert it back at index 0.
		* Optionally, reconstruct the element to it's default state.
		*
		* @param Reconstruct - Reconstruct all elements inside the container to it's default state. [Default] true.
		*/
		void Empty(bool Reconstruct = true)
		{
			Destruct(0, Len, Reconstruct);
			Len = 0;
		}

		/**
		* Shrinks the size of the Array to perfectly fit it's contents.
		* This means storing only enough space to hold the total elements inside the Array.
		* Returns true on success, false on error.
		*/
		bool ShrinkToFit() 
		{
			ElementType* TempData = nullptr;

			Destruct(Len, Capacity);
			size_t Length = (Len <= Capacity) ? Len : Capacity;

			if (Length < Len) 
			{
				TempData = (ElementType*)realloc(Data, sizeof(ElementType) * Len);

				if (!TempData)
					return false;

				Data = TempData;
			}

			return true;
		}

		/**
		* Returns the index of the specified object in the Array by reference.
		* Not to be used for Arrays storing pointers. Check Find() function.
		*
		* @param Element - An element passed in as value.
		*/
		inline size_t IndexOf(const ElementType& Element) const 
		{
			return IndexOf(&Element);
		}

		/**
		* Returns the index of the specified object in the Array.
		* Not to be used for Arrays storing pointers. Check Find() function.
		*
		* @param Element - An element passed in by pointer.
		*/
		inline size_t IndexOf(const ElementType* Element) const 
		{
			// Object must reside within the boundaries of the Array.
			_ASSERTE(Element >= Data && Element < Data + Capacity);

			// The specified object should reside at an address that is an even multiple of of the Type's size.
			_ASSERTE(((unsigned __int8*)Element - (unsigned __int8*)Data) % sizeof(ElementType) == 0);

			return (size_t)(Element - Data);
		}

		/**
		* Retrieves the index of the element in the Array.
		* Returns a -1 if not found.
		* If there are multiple similar elements
		*
		* @param Element - Object to be evaluated.
		*/
		inline size_t Find(const ElementType& Element) const 
		{
			for (size_t i = 0; i < Len; i++)
				if (Element == Data[i])
					return i;

			return -1;
		}

		/**
		* Iterates the Array and checks if the specified element exists.
		* Returns true if exist, false if not.
		*
		* @param Element - Object to be evaluated.
		*/
		inline bool Has(const ElementType& Element) const 
		{
			return Find(Element) != -1;
		}

		/**
		* Returns the index of the end-most object within the Array.
		*/
		inline size_t Length() const 
		{
			return Len;
		}

		/**
		* Returns the internal buffer size allocated inside the Array.
		*/
		inline size_t Size() const 
		{
			return Capacity;
		}

		/**
		* Returns a pointer to the first element in the Array.
		*/
		inline ElementType* First() 
		{
			return Data;
		}


		/**
		* Returns a pointer to the last element in the Array.
		* If length of array is 0, Last() returns the 0th element.
		*/
		inline ElementType* Last()
		{
			return Data + Len - 1;
		}


		/**
		* Returns a reference to the first element in the Array.
		*/
		inline ElementType& Front() 
		{
			return *Data;
		}

		/**
		* Returns a reference to the first element in the Array.
		* If the length of the Array is 0, Back() returns the 0th element.
		*/
		inline ElementType& Back() 
		{
			return *Last();
		}

		/**
		* Range-based loop initialiser.
		*/
		Iterator begin() const 
		{
			return Iterator(Data);
		}

		/**
		* Range-based loop terminator.
		*/
		Iterator end() const 
		{
			return Iterator(Data + Len);
		}

		/**
		* Reverse iterator begin.
		*/
		Iterator rbegin() const 
		{
			return Iterator(Data + Len);
		}

		/**
		* Reverse iterator end.
		*/
		Iterator rend() const 
		{
			return Iterator(Data);
		}

	};


}


#endif // !GRAVITY_LIB_ARRAY