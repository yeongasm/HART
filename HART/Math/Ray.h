#pragma once
#include "Vector.h"


namespace hart
{

	template <typename T>
	struct Ray
	{
		Vec3<T> Origin;
		Vec3<T> Direction;

		Ray() :
			Origin(), Direction() {}

		~Ray()
		{
			Origin = Direction = 0;
		}

		Ray(const Ray& Rhs)
		{
			*this = Rhs;
		}

		Ray& operator= (const Ray& Rhs)
		{
			if (this != &Rhs)
			{
				Origin = Rhs.Origin;
				Direction = Rhs.Direction;
			}

			return *this;
		}

		Ray(Ray&& Rhs)
		{
			*this = Gfl::Move(Rhs);
		}

		Ray& operator= (Ray&& Rhs)
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs)
			{
				Origin = Gfl::Move(Rhs.Origin);
				Direction = Gfl::Move(Rhs.Direction);

				new (&Rhs) Ray();
			}

			return *this;
		}

		Ray(Vec3<T> Origin, Vec3<T> Direction) :
			Origin(Origin), Direction(Direction) {}

		Vec3<T> PointAt(T Value) const
		{
			return Origin + (Value * Direction);
		}
	};

}