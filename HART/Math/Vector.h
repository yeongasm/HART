#pragma once
#include <Library/GrvtTemplates.h>

namespace hart
{

	template <typename T>
	class Vec3
	{
	public:

		static const Vec3 Right;
		static const Vec3 Left;
		static const Vec3 Up;
		static const Vec3 Down;
		static const Vec3 Front;
		static const Vec3 Back;

		union
		{
			T Data[3];

			struct
			{
				T x;
				T y;
				T z;
			};

			struct
			{
				T r;
				T g;
				T b;
			};
		};

		Vec3() :
			x(0), y(0), z(0) {}

		~Vec3()
		{
			x = y = z = 0;
		}

		Vec3(const Vec3& Rhs)
		{
			*this = Rhs;
		}

		Vec3& operator= (const Vec3& Rhs)
		{
			if (this != &Rhs)
			{
				x = Rhs.x;
				y = Rhs.y;
				z = Rhs.z;
			}

			return *this;
		}

		Vec3(Vec3&& Rhs)
		{
			*this = Gfl::Move(Rhs);
		}

		Vec3& operator= (Vec3&& Rhs)
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs)
			{
				x = Rhs.x;
				y = Rhs.y;
				z = Rhs.z;

				new (&Rhs) Vec3();
			}

			return *this;
		}

		Vec3(T Value) :
			x(Value), y(Value), z(Value) {}

		Vec3(T X, T Y, T Z) :
			x(X), y(Y), z(Z) {}

		T operator[] (size_t Index)
		{
			static_assert(Index > 2);

			return Data[Index];
		}

		const T operator[] (size_t Index) const
		{
			static_assert(Index > 2);

			return Data[Index];
		}

		T Length() const
		{
			return static_cast<T>(sqrt((x * x) + (y * y) + (z * z)));
		}

		T SquaredLength() const
		{
			return x * x + y * y + z * z;
		}

		Vec3 Normalize() const
		{
			T Norm = 1 / Length();

			return Vec3(x * Norm, y * Norm, z * Norm);
		}

		T Dot(const Vec3& Rhs) const
		{
			return x * Rhs.x + y * Rhs.y + z * Rhs.z;
		}

		Vec3 Cross(const Vec3& Rhs) const
		{
			return Vec3(y * Rhs.z - z * Rhs.y,
				z * Rhs.x - x * Rhs.z,
				x * Rhs.y - y * Rhs.x);
		}

		Vec3& operator= (T Scalar)
		{
			x = y = z = Scalar;

			return *this;
		}

		Vec3 operator+ (const Vec3& Rhs) const
		{
			return Vec3(x + Rhs.x, y + Rhs.y, z + Rhs.z);
		}

		Vec3 operator+ (T Scalar) const
		{
			return Vec3(x + Scalar, y + Scalar, z + Scalar);
		}

		Vec3 operator- (const Vec3& Rhs) const
		{
			return Vec3(x - Rhs.x, y - Rhs.y, z - Rhs.z);
		}

		Vec3 operator- (T Scalar) const
		{
			return Vec3(x - Scalar, y - Scalar, z - Scalar);
		}

		Vec3 operator* (const Vec3& Rhs) const
		{
			return Vec3(x * Rhs.x, y * Rhs.y, z * Rhs.z);
		}

		Vec3 operator* (T Scalar) const
		{
			return Vec3(x * Scalar, y * Scalar, z * Scalar);
		}

		Vec3 operator/ (const Vec3& Rhs) const
		{
			return Vec3(x / Rhs.x, y / Rhs.y, z / Rhs.z);
		}

		Vec3 operator/ (T Scalar) const
		{
			return Vec3(x / Scalar, y / Scalar, z / Scalar);
		}

		Vec3& operator+= (const Vec3& Rhs)
		{
			x += Rhs.x;
			y += Rhs.y;
			z += Rhs.z;

			return *this;
		}

		Vec3& operator+= (T Scalar)
		{
			x += Scalar;
			y += Scalar;
			z += Scalar;

			return *this;
		}

		Vec3& operator-= (const Vec3& Rhs)
		{
			x -= Rhs.x;
			y -= Rhs.y;
			z -= Rhs.z;

			return *this;
		}

		Vec3& operator-= (T Scalar)
		{
			x -= Scalar;
			y -= Scalar;
			z -= Scalar;

			return *this;
		}

		Vec3& operator*= (const Vec3& Rhs)
		{
			x *= Rhs.x;
			y *= Rhs.y;
			z *= Rhs.z;

			return *this;
		}

		Vec3& operator*= (T Scalar)
		{
			x *= Scalar;
			y *= Scalar;
			z *= Scalar;

			return *this;
		}

		Vec3& operator/= (const Vec3& Rhs)
		{
			x /= Rhs.x;
			y /= Rhs.y;
			z /= Rhs.z;

			return *this;
		}

		Vec3& operator/= (T Scalar)
		{
			x /= Scalar;
			y /= Scalar;
			z /= Scalar;

			return *this;
		}
	};

	using vec3 = Vec3<float>;
	using rgb = Vec3<float>;

	template <typename T>
	Vec3<T> Normalize(const Vec3<T>& Vector)
	{
		return Vector.Normalize();
	}

	template <typename T>
	inline Vec3<T> operator+ (T Scalar, const Vec3<T>& Vector)
	{
		return Vec3<T>(Vector.x + Scalar, Vector.y + Scalar, Vector.z + Scalar);
	}

	template <typename T>
	inline Vec3<T> operator- (T Scalar, const Vec3<T>& Vector)
	{
		return Vec3<T>(Vector.x - Scalar, Vector.y - Scalar, Vector.z - Scalar);
	}

	template <typename T>
	inline Vec3<T> operator- (const Vec3<T>& Vector)
	{
		return Vec3<T>(-Vector.x, -Vector.y, -Vector.z);
	}

	template <typename T>
	inline Vec3<T> operator* (T Scalar, const Vec3<T>& Vector)
	{
		return Vec3<T>(Vector.x * Scalar, Vector.y * Scalar, Vector.z * Scalar);
	}

	template <typename T>
	inline Vec3<T> operator/ (T Scalar, const Vec3<T>& Vector)
	{
		return Vec3<T>(Vector.x / Scalar, Vector.y / Scalar, Vector.z / Scalar);
	}

	template <typename T>
	inline T Dot(const Vec3<T>& First, const Vec3<T>& Second)
	{
		return First.Dot(Second);
	}

	template <typename T>
	inline Vec3<T> Cross(const Vec3<T>& First, const Vec3<T>& Second)
	{
		return First.Cross(Second);
	}

	template <typename T>
	inline Vec3<T> Reflect(const Vec3<T>& InVec, const Vec3<T>& Normal)
	{
		return InVec - 2 * Dot(InVec, Normal) * Normal;
	}

	template <typename T>
	inline bool Refract(const Vec3<T>& InVec, const Vec3<T>& Normal, const T N1, const T N2, Vec3<T>& OutVec)
	{
		const T N = N1 / N2;
		const T CosI = -Dot(InVec, Normal);
		const T SinT2 = N * N * (static_cast<T>(1) - CosI * CosI);

		if (SinT2 > static_cast<T>(1))
		{
			return false;
		}

		const T CosT = static_cast<T>(sqrt(static_cast<double>(1) - static_cast<double>(SinT2)));
		OutVec = N * InVec + (N * CosI - CosT) * Normal;

		return true;
	}


	template <typename T>
	Vec3<T> Lerp(const Vec3<T>& A, const Vec3<T>& B, T V)
	{
		return (1 - V) * A + (V * B);
	}

	
	template <typename T>
	Vec3<T> Mix(const Vec3<T>& A, const Vec3<T>& B, T V)
	{
		return Lerp(A, B, V);
	}


#ifndef HART_GLOBAL_VECTORS
#define HART_GLOBAL_VECTORS

	template <typename T> const Vec3<T> Vec3<T>::Right(1, 0, 0);
	template <typename T> const Vec3<T> Vec3<T>::Left(-1, 0, 0);
	template <typename T> const Vec3<T> Vec3<T>::Up(0, 1, 0);
	template <typename T> const Vec3<T> Vec3<T>::Down(0, -1, 0);
	template <typename T> const Vec3<T> Vec3<T>::Front(0, 0, 1);
	template <typename T> const Vec3<T> Vec3<T>::Back(0, 0, -1);

#endif // !HART_GLOBAL_VECTORS;

}