#pragma once

#include "Threading/JobObject.h"
#include "Math/Geometry.h"
#include "RayTracing/Camera.h"
#include "RayTracing/Xoroshiro.h"
#include "RayTracing/Material.h"

namespace hart
{

	struct RayCastArgs
	{
		HartCamera*		CameraPtr	= nullptr;
		GeometryList*	ScenePtr	= nullptr;
		Gfl::uint8*		PixelPtr	= nullptr;
		size_t			Index		= 0;
		Gfl::uint32		Samples		= 0;
		Gfl::uint32		V			= 0;
		Gfl::uint32		H			= 0;
		Gfl::uint32		Width		= 0;
		Gfl::uint32		Height		= 0;
	};


	/**
	* The Rasterize function represents the lifecycle of a SINGLE ray!
	* Hence all HitRecord really does is caching the results of where the ray has hit. That is all and nothing else.
	*/
	rgb Rasterize(const Ray<float>& RayCast, const GeometryList* ScenePtr, Gfl::uint32 Depth, Rng32& PRNG, float CurrentHeight, float MaxHeight)
	{
		HitRecord Record;

		float Radiance = 1.0f;

		// OnTick returns true if a ray has hit a geometry.
		if (ScenePtr->OnTick(RayCast, 0.0f, FLT_MAX, Record))
		{
			Ray<float> ScatteredRay;
			vec3 Attenuation;
			vec3 Emission = Record.MatPtr->Emitted(Record.U, Record.V, Record);

			if (Depth < 5 && Record.MatPtr->Scatter(RayCast, Record, Attenuation, ScatteredRay, PRNG))
			{
				return Emission + Attenuation * Rasterize(ScatteredRay, ScenePtr, Depth + 1, PRNG, CurrentHeight, MaxHeight);
			}

			return Emission;
		}

		return rgb(0.0f);

		//return rgb(1.0f);

		//vec3 UnitVector = RayCast.Direction.Normalize();

		// Interpolate background from blue to white.
		//float t = 0.5f * UnitVector.y + 1.0f;

		//return (1.0f - t) * rgb(1.0f, 1.0f, 1.0f) + t * rgb(0.57f, 0.57f, 0.57f);

		//return (1.0f - t) * rgb(1.0f, 1.0f, 1.0f) + t * rgb(0.5f, 0.7f, 1.0f);
	}


	void RayCast(void* Param)
	{
		RayCastArgs* Args = (RayCastArgs*)Param;

		const float Gamma = 2.2f;
		rgb Colour(0.0f, 0.0f, 0.0f);

		// Experiment seeding xoroshiro PRNG with it's own Index.
		Rng32 Rand(Args->Index);

		for (Gfl::uint32 k = 0; k < Args->Samples; k++)
		{
			float u = ((float)Args->H + UnitRand<float>(Rand)) / (float)Args->Width;
			float v = ((float)Args->V + UnitRand<float>(Rand)) / (float)Args->Height;

			Ray<float> Ray = Args->CameraPtr->GetRay(u, v, Rand);

			Colour += Rasterize(Ray, Args->ScenePtr, 0, Rand, (float)Args->V, (float)Args->Height);
		}

		Colour /= (float)Args->Samples;
		Colour = rgb(sqrtf(Colour.r), sqrtf(Colour.g), sqrtf(Colour.b));

		Colour.r = Gfl::Min(1.0f, Colour.r);
		Colour.g = Gfl::Min(1.0f, Colour.g);
		Colour.b = Gfl::Min(1.0f, Colour.b);

		Args->PixelPtr[Args->Index + 0] = (Gfl::uint8)(255.99f * Colour.r);
		Args->PixelPtr[Args->Index + 1] = (Gfl::uint8)(255.99f * Colour.g);
		Args->PixelPtr[Args->Index + 2] = (Gfl::uint8)(255.99f * Colour.b);
		Args->PixelPtr[Args->Index + 3] = (Gfl::uint8)(255.99f * 1.0f);
	}
}