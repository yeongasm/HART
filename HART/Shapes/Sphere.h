#pragma once

#include <cmath>
#include <Math/Geometry.h>
#include <RayTracing/Xoroshiro.h>

namespace hart
{

	struct Sphere : public BaseGeometry
	{
		vec3	Center;
		float	Radius;

		Sphere() :
			BaseGeometry(), Center(0.0f), Radius(0.0f) {}

		~Sphere()
		{
			Radius = 0.0f;
			Center = vec3(0.0f);
		}

		Sphere(vec3 Center, float Radius, BaseMaterial* AssignedMaterial) :
			BaseGeometry(AssignedMaterial), Center(Center), Radius(Radius) {}

		bool Hit(const Ray<float>& Ray, float TMin, float TMax, HitRecord& Record) const
		{
			vec3 Distance = Ray.Origin - Center;

			float a = Dot(Ray.Direction, Ray.Direction);
			float b = Dot(Distance, Ray.Direction);
			float c = Dot(Distance, Distance) - Radius * Radius;

			float Discriminant = b * b - a * c;

			if (Discriminant > 0.0f)
			{
				float Temp = (-b - sqrt(Discriminant)) / a;

				if (Temp < TMax && Temp > TMin)
				{
					Record.MatPtr = this->MatPtr;
					Record.T = Temp;
					Record.Point = Ray.PointAt(Temp);
					Record.Normal = (Record.Point - Center) / Radius;

					return true;
				}

				Temp = (-b + sqrt(Discriminant)) / a;

				if (Temp < TMax && Temp > TMin)
				{
					Record.MatPtr = this->MatPtr;
					Record.T = Temp;
					Record.Point = Ray.PointAt(Temp);
					Record.Normal = (Record.Point - Center) / Radius;

					return true;
				}
			}

			return false;
		}
	};


	vec3 RandomPointInUnitSphere(Rng32& PRNG)
	{
		vec3 Point;

		// A point is said to be on the as long as sum of the squared of each component does not exceed it's radius squared.
		// x^2 + y^2 <= r^2
		// The same is true for a sphere where:
		// x^2 + y^2 + z^2 <= r^2
		do
		{
			vec3 Random = vec3(UnitRand<float>(PRNG), UnitRand<float>(PRNG), UnitRand<float>(PRNG));
			Point = 2.0f * Random - vec3(1.0f, 1.0f, 1.0f);
		} 
		while (Point.SquaredLength() >= 1.0f);

		return Point;
	}

}