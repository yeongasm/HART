#pragma once
#ifndef GRAVITY_LIB_TEMPLATES
#define GRAVITY_LIB_TEMPLATES


namespace Gfl 
{

	template<typename T>
	struct TRemoveReference
	{
		using Type = T;
	};


	template <typename T>
	struct TRemoveReference<T&>
	{
		using Type = T;
	};


	template <typename T>
	struct TRemoveReference<T&&>
	{
		using Type = T;
	};

	
	template <typename T>
	using RemoveReference = typename TRemoveReference<T>::Type;


	template<typename T>
	struct TRemovePointer
	{
		using Type = T;
	};


	template<typename T>
	struct TRemovePointer<T*>
	{
		using Type = T;
	};


	template <typename T>
	using RemovePointer = typename TRemovePointer<T>::Type;


	template <typename A, typename B>
	struct TAreTypesEqual;

	
	template <typename, typename>
	struct TAreTypesEqual
	{
		enum { Value = false };
	};


	template <typename A>
	struct TAreTypesEqual<A, A>
	{
		enum { Value = true };
	};


	template <typename T>
	struct TIsPointer
	{
		enum { Value = false };
	};


	template <typename T>
	struct TIsPointer<T*>
	{
		enum { Value = true };
	};


	template <typename T>
	struct TIsPointer<const T>
	{
		enum { Value = TIsPointer<T>::Value };
	};


	template <typename T>
	struct TIsPointer<volatile T>
	{
		enum {
			Value = TIsPointer<T>::Value
		};
	};


	template <typename T>
	struct TIsPointer<const volatile T>
	{
		enum {
			Value = TIsPointer<T>::Value
		};
	};


	template <typename Type>
	Type&& Forward(RemoveReference<Type>& Object) noexcept
	{
		return static_cast<Type&&>(Object);
	}


	template <typename Type>
	Type&& Forward(RemoveReference<Type>&& Object) noexcept
	{
		return static_cast<Type&&>(Object);
	}

	template <typename Type>
	RemoveReference<Type>&& Move(Type&& Object) noexcept
	{
		using CastType = typename TRemoveReference<Type>::Type;
		static_assert(!TAreTypesEqual<CastType&, const CastType&>::Value, "Move semantics called on a const object");
		return static_cast<CastType&&>(Object);
	}

}


#endif // !GRAVITY_TEMPLATES