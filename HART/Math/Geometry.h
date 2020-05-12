#pragma once

#include <Library/GrvtArray.h>
#include "Ray.h"

namespace hart
{

	struct BaseMaterial;


	/**
	* Point		- Represents the point on the geometry where the ray has hit.
	* Normal	- Surface normal of the geometry where the ray hit.
	* MatPtr	- Geometry's assigned material.
	*/
	struct HitRecord
	{
		float			T;
		float			U;
		float			V;
		vec3			Point;
		vec3			Normal;
		BaseMaterial*	MatPtr;
	};


	struct BaseGeometry
	{
		BaseMaterial* MatPtr;

		BaseGeometry() :
			MatPtr(nullptr) {}

		/**
		* TODO(Afiq):
		* For now we'll use RAII but in the future, it's best to have a manager that handles all materials.
		*/
		virtual ~BaseGeometry()
		{
			if (MatPtr)
			{
				delete MatPtr;
			}
		}

		BaseGeometry(BaseMaterial* AssignedMaterial) :
			MatPtr(AssignedMaterial) {}

		virtual bool Hit(const Ray<float>& Ray, float TMin, float TMax, HitRecord& Record) const = 0;
	};


	class GeometryList
	{
	private:

		Gfl::Array<BaseGeometry*> Geometries;

	public:

		GeometryList() :
			Geometries() {}

		~GeometryList() {}

		bool PushGeometry(BaseGeometry* Shape)
		{
			size_t Index = Geometries.Push(Shape);

			return Index != -1;
		}


		/**
		* What OnTick really does it just tell the program if a piece of geometry has been hit by a ray.
		*/
		bool OnTick(const Ray<float>& SourceRay, float TMin, float TMax, HitRecord& Record) const
		{
			HitRecord TempRecord;

			bool HitAnything  = false;

			float ClosestSoFar = TMax;

			for (BaseGeometry* Geometry : Geometries)
			{
				if (Geometry->Hit(SourceRay, TMin, ClosestSoFar, TempRecord))
				{
					HitAnything = true;
					ClosestSoFar = TempRecord.T;
					Record = TempRecord;
				}
			}
			
			return HitAnything;
		}


		void Free()
		{
			for (size_t i = 0; i < Geometries.Length(); i++)
			{
				delete Geometries[i];
				Geometries[i] = nullptr;
			}

			Geometries.Release();
		}
	};

}