#pragma once
#include <initializer_list>
#include <Library/GrvtTemplates.h>
#include "Vector.h"

namespace hart
{


	enum class MatTypeIdentity
	{
		Mat_Identity
	};

	enum class MatTypeZero
	{
		Mat_Zero
	};


	template <typename T>
	class Matrix3
	{
	public:

		T m00, m01, m02;
		T m10, m11, m12;
		T m20, m21, m22;

		Matrix3() :
			m00(0), m01(0), m02(0), m10(0), m11(0), m12(0), m20(0), m21(0), m22(0) {}

		~Matrix3() {}

		Matrix3(T x00, T x01, T x02, T x10, T x11, T x12, T x20, T x21, T x22) :
			m00(x00), m01(x01), m02(x02), m10(x10), m11(x11), m12(x12), m20(x20), m21(x21), m22(x22) {}

		Matrix3(const Vec3<T>& Vi, const Vec3<T>& Vj, const Vec3<T>& Vk)
		{
			m00 = T(Vi.x);
			m01 = T(Vi.y);
			m02 = T(Vi.z);
			m10 = T(Vj.x);
			m11 = T(Vj.y);
			m12 = T(Vj.z);
			m20 = T(Vk.x);
			m21 = T(Vk.y);
			m22 = T(Vk.z);
		}


		Matrix3(MatTypeIdentity)
		{
			m00 = 1;
			m01 = 0;
			m02 = 0;
			m10 = 0;
			m11 = 1;
			m12 = 0;
			m20 = 0;
			m21 = 0;
			m22 = 1;
		}

		Matrix3(MatTypeZero)
		{
			m00 = 0;
			m01 = 0;
			m02 = 0;
			m10 = 0;
			m11 = 0;
			m12 = 0;
			m20 = 0;
			m21 = 0;
			m22 = 0;
		}


		Matrix3(const Matrix3& Rhs)
		{
			*this = Rhs;
		}


		Matrix3& operator= (const Matrix3& Rhs)
		{
			if (this != &Rhs)
			{
				m00 = Rhs.m00;
				m01 = Rhs.m01;
				m02 = Rhs.m02;
				m10 = Rhs.m01;
				m11 = Rhs.m11;
				m12 = Rhs.m12;
				m20 = Rhs.m20;
				m21 = Rhs.m21;
				m22 = Rhs.m22;
			}

			return *this;
		}


		Matrix3(Matrix3&& Rhs)
		{
			*this = Gfl::Move(Rhs);
		}


		Matrix3& operator= (Matrix3&& Rhs)
		{
			if (this != &Rhs)
			{
				*this = Rhs;

				new (&Rhs) Matrix3();
			}

			return *this;
		}


		Matrix3 operator+ (T Value) const
		{
			Matrix3 Out;

			Out.m00 = m00 + Value;
			Out.m01 = m01 + Value;
			Out.m02 = m02 + Value;
			Out.m10 = m10 + Value;
			Out.m11 = m11 + Value;
			Out.m12 = m12 + Value;
			Out.m20 = m20 + Value;
			Out.m21 = m21 + Value;
			Out.m22 = m22 + Value;

			return Out;
		}


		Matrix3& operator+= (T Value)
		{
			m00 += Value;
			m01 += Value;
			m02 += Value;
			m10 += Value;
			m11 += Value;
			m12 += Value;
			m20 += Value;
			m21 += Value;
			m22 += Value;

			return *this;
		}


		Matrix3 operator- (T Value) const
		{
			Matrix3 Out;

			Out.m00 = m00 - Value;
			Out.m01 = m01 - Value;
			Out.m02 = m02 - Value;
			Out.m10 = m10 - Value;
			Out.m11 = m11 - Value;
			Out.m12 = m12 - Value;
			Out.m20 = m20 - Value;
			Out.m21 = m21 - Value;
			Out.m22 = m22 - Value;

			return Out;
		}


		Matrix3& operator-= (T Value)
		{
			m00 -= Value;
			m01 -= Value;
			m02 -= Value;
			m10 -= Value;
			m11 -= Value;
			m12 -= Value;
			m20 -= Value;
			m21 -= Value;
			m22 -= Value;

			return *this;
		}


		Matrix3 operator* (T Value) const
		{
			Matrix3 Out;

			Out.m00 = m00 * Value;
			Out.m01 = m01 * Value;
			Out.m02 = m02 * Value;
			Out.m10 = m10 * Value;
			Out.m11 = m11 * Value;
			Out.m12 = m12 * Value;
			Out.m20 = m20 * Value;
			Out.m21 = m21 * Value;
			Out.m22 = m22 * Value;

			return Out;
		}


		Matrix3& operator*= (T Value)
		{
			m00 *= Value;
			m01 *= Value;
			m02 *= Value;
			m10 *= Value;
			m11 *= Value;
			m12 *= Value;
			m20 *= Value;
			m21 *= Value;
			m22 *= Value;

			return *this;
		}

		
		Matrix3 operator/ (T Value) const
		{
			Matrix3 Out;

			T Val = 1 / Value;

			Out.m00 = m00 * Val;
			Out.m01 = m01 * Val;
			Out.m02 = m02 * Val;
			Out.m10 = m10 * Val;
			Out.m11 = m11 * Val;
			Out.m12 = m12 * Val;
			Out.m20 = m20 * Val;
			Out.m21 = m21 * Val;
			Out.m22 = m22 * Val;

			return Out;
		}


		Matrix3& operator/= (T Value)
		{
			T Val = 1 / Value;

			m00 *= Val;
			m01 *= Val;
			m02 *= Val;
			m10 *= Val;
			m11 *= Val;
			m12 *= Val;
			m20 *= Val;
			m21 *= Val;
			m22 *= Val;

			return *this;
		}

		
		Matrix3 operator+ (const Matrix3& Rhs) const
		{
			Matrix3 Out;

			Out.m00 = m00 + Rhs.m00;
			Out.m01 = m01 + Rhs.m01;
			Out.m02 = m02 + Rhs.m02;
			Out.m10 = m10 + Rhs.m10;
			Out.m11 = m11 + Rhs.m11;
			Out.m12 = m12 + Rhs.m12;
			Out.m20 = m20 + Rhs.m20;
			Out.m21 = m21 + Rhs.m21;
			Out.m22 = m22 + Rhs.m22;

			return Out;
		}


		Matrix3& operator+= (const Matrix3& Rhs)
		{
			m00 += Rhs.m00;
			m01 += Rhs.m01;
			m02 += Rhs.m02;
			m10 += Rhs.m10;
			m11 += Rhs.m11;
			m12 += Rhs.m12;
			m20 += Rhs.m20;
			m21 += Rhs.m21;
			m22 += Rhs.m22;

			return *this;
		}

		
		Matrix3 operator- (const Matrix3& Rhs) const
		{
			Matrix3 Out;

			Out.m00 = m00 - Rhs.m00;
			Out.m01 = m01 - Rhs.m01;
			Out.m02 = m02 - Rhs.m02;
			Out.m10 = m10 - Rhs.m10;
			Out.m11 = m11 - Rhs.m11;
			Out.m12 = m12 - Rhs.m12;
			Out.m20 = m20 - Rhs.m20;
			Out.m21 = m21 - Rhs.m21;
			Out.m22 = m22 - Rhs.m22;

			return Out;
		}


		Matrix3& operator-= (const Matrix3& Rhs)
		{
			m00 -= Rhs.m00;
			m01 -= Rhs.m01;
			m02 -= Rhs.m02;
			m10 -= Rhs.m10;
			m11 -= Rhs.m11;
			m12 -= Rhs.m12;
			m20 -= Rhs.m20;
			m21 -= Rhs.m21;
			m22 -= Rhs.m22;

			return *this;
		}


		Matrix3 operator* (const Matrix3& Rhs) const
		{
			Matrix3 Out;

			Out.m00 = m00 * Rhs.m00 + m01 * Rhs.m10 + m02 * Rhs.m20;
			Out.m01 = m00 * Rhs.m01 + m01 * Rhs.m11 + m02 * Rhs.m21;
			Out.m02 = m00 * Rhs.m02 * m01 * Rhs.m12 + m02 * Rhs.m22;
			Out.m10 = m10 * Rhs.m00 + m11 * Rhs.m10 + m12 * Rhs.m20;
			Out.m11 = m10 * Rhs.m01 + m11 * Rhs.m11 + m12 * Rhs.m21;
			Out.m12 = m10 * Rhs.m02 * m11 * Rhs.m12 + m12 * Rhs.m22;
			Out.m20 = m20 * Rhs.m00 + m21 * Rhs.m10 + m22 * Rhs.m20;
			Out.m21 = m20 * Rhs.m01 + m21 * Rhs.m11 + m22 * Rhs.m21;
			Out.m22 = m20 * Rhs.m02 * m21 * Rhs.m12 + m22 * Rhs.m22;

			return Out;
		}


		T Determinant() const
		{
			return (m00 * m11 * m22) + (m01 * m12 * m20) + (m02 * m10 * m21) - (m02 * m11 * m20) - (m01 * m10 * m22) - (m00 * m12 * m21);
		}


		/**
		* Inverts itself.
		* Not sure if this is correct. Will need to study more about inverting a matrix.
		*/
		bool Invert()
		{
			// Rescue members before change.
			Matrix3 m = *this;

			// Calculate cofactor (transposed adjoined matrix)
			m00 = m.m22 * m.m11 - m.m12 * m.m21;
			m01 = m.m02 * m.m21 - m.m22 * m.m01;
			m02 = m.m12 * m.m01 - m.m02 * m.m11;
			m10 = m.m12 * m.m20 - m.m22 * m.m10;
			m11 = m.m22 * m.m00 - m.m02 * m.m20;
			m12 = m.m02 * m.m10 - m.m12 * m.m00;
			m20 = m.m10 * m.m21 - m.m20 * m.m11;
			m21 = m.m20 * m.m01 - m.m00 * m.m21;
			m22 = m.m00 * m.m11 - m.m10 * m.m01;

			T Det = (m.m00 * m00 + m.m10 * m01 + m.m20 * m02);

			if (Det < 1E-40f)
			{
				return 0;
			}

			T IDet = (T)1.0 / Det;

			*this *= IDet;

			return 1;
		}


		/**
		* Returns the inverse of this matrix.
		*/
		Matrix3 Inverse() const
		{
			Matrix3 Out = *this;

			Out.Invert();

			return Out;
		}


		/**
		* Transposes itself.
		*/
		Matrix3& Transposed()
		{
			T Temp;

			Temp = m01;
			m01 = m10;
			m10 = Temp;

			Temp = m02;
			m02 = m20;
			m20 = Temp;

			Temp = m12;
			m12 = m21;
			m21 = Temp;

			return *this;
		}


		/**
		* Returns the transpose of the matrix.
		*/
		Matrix3 Transpose() const
		{
			Matrix3 Out = *this;

			Out.Transposed();

			return Out;
		}
	};


	template <typename T>
	Vec3<T> operator* (const Matrix3<T>& M, const Vec3<T>& V)
	{
		Vec3<T> Out;

		Out.x = T(V.x * M.m00 + V.y * M.m01 + V.z * M.m02);
		Out.y = T(V.x * M.m10 + V.y * M.m11 + V.z * M.m12);
		Out.z = T(V.x * M.m20 + V.y * M.m21 + V.z * M.m22);

		return Out;
	}


	template <typename T>
	Vec3<T> operator* (const Vec3<T>& V, const Matrix3<T>& M)
	{
		Vec3<T> Out;

		Out.x = T(V.x * M.m00 + V.y * M.m10 + V.z * M.m20);
		Out.y = T(V.x * M.m01 + V.y * M.m11 + V.z * M.m21);
		Out.z = T(V.x * M.m02 + V.y * M.m12 + V.z * M.m22);

		return Out;
	}

	using mat3 = Matrix3<float>;

}