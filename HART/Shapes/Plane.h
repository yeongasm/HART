#pragma once

#include <cmath>
#include <Math/Geometry.h>
#include <Math/Matrix.h>


namespace hart
{

	/**
	* For the sake of simplicity, a plane will be defined as 
	*/
	struct Parallelogram : public BaseGeometry
	{
		vec3	BL;
		vec3	E1;
		vec3	E2;
		float	Width;
		float	Length;

		Parallelogram() :
			BaseGeometry(), BL(0.0f), E1(0.0f), E2(0.0f), Width(0.0f), Length(0.0f) {}

		~Parallelogram() {}

		Parallelogram(vec3 BottomLeft, vec3 E_1, vec3 E_2, float Width, float Length, BaseMaterial* AssignedMaterial) :
			BaseGeometry(AssignedMaterial), BL(BottomLeft), Width(Width), Length(Length) 
		{
			E1 = Normalize(E_1);
			E2 = Normalize(E_2);
		}


		bool Hit(const Ray<float>& Ray, float TMin, float TMax, HitRecord& Record) const
		{
			vec3 Distance = Ray.Origin - BL;

			vec3 r1 = -Ray.Direction;
			//mat3 Mat(-Ray.Direction, E1, E2);

			//Mat.Invert();

			//vec3 Result = Distance * Mat;

			//float T = Result.x;
			//float U = Result.y;
			//float V = Result.z;

			const float Denom = Dot(r1, Cross(E1, E2));

			float T = (Dot(Cross(E1, E2), Distance) / Denom);
			float U = (Dot(Cross(E2, r1), Distance) / Denom);
			float V = (Dot(Cross(r1, E1), Distance) / Denom);

			if (T < 0.0f)
			{
				return false;
			}

			U *= (1.0f / Width);
			V *= (1.0f / Length);
			
			if ((U < 0.0f || U > 1.0f) || (V < 0.0f || V > 1.0f))
			{
				return false;
			}

			if (T < TMax && T > TMin)
			{
				Record.MatPtr = this->MatPtr;
				Record.T = T;
				Record.Point = Ray.PointAt(T);
				Record.Normal = Cross(E1, E2);
				Record.U = U;
				Record.V = V;

				return true;
			}

			return false;
		}
	};

}