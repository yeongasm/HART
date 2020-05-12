#pragma once

#include <Math/Ray.h>

namespace hart
{

	template <typename T = double>
	constexpr T Pi()
	{
		return static_cast<T>(4 * atan(1));
	}


	struct CameraConfig
	{
		vec3	LookFrom;
		vec3	LookTo;
		float	Distance;
		float	ViewAngle;
		float	Aperture;
	};


	class HartCamera
	{
	private:

		// These values represent the dimensions of the image plane.
		// The value of the TL value would be half of the vertical and horizontal value for the aspect ratio.
		vec3 TL;

		// Horizontal and vertical values for the aspect ratio.
		vec3 Horizontal;
		vec3 Vertical;

		vec3 Origin;

		vec3 U;
		vec3 V;
		vec3 W;

		float LensRadius;

		vec3 RandomInUnitDisk(Rng32& PRNG) const
		{
			vec3 Point;

			do
			{
				Point = 2.0f * vec3(UnitRand<float>(PRNG), UnitRand<float>(PRNG), 0.0f) - vec3(1.0f, 1.0f, 0.0f);
			} 
			while (Dot(Point, Point) > 1.0f);

			return Point;
		}

	public:

		HartCamera(vec3 LookFrom, vec3 LookAt, float VerticalFieldOfView, int Width, int Height, float Aperture, float FocusDistance)
		{
			const float AspectRatio = (float)Width / (float)Height;
			const float Theta = VerticalFieldOfView * Pi<float>() / 180.0f;
			const float HalfHeight = tan(Theta / 2.0f);
			const float HalfWidth = AspectRatio * HalfHeight;

			LensRadius = Aperture / 2.0f;
			
			Origin = LookFrom;

			W = Normalize(LookFrom - LookAt);					// Similar to Z axis
			U = Normalize(Cross(vec3(0.0f, 1.0f, 0.0f), W));	// Similar to X axis
			V = Normalize(Cross(W, U));							// Similar to Y axis.

			TL = Origin - HalfWidth * FocusDistance * U + HalfHeight * FocusDistance * V - FocusDistance * W;

			Horizontal	= 2.0f * HalfWidth * FocusDistance * U;
			Vertical	= 2.0f * HalfHeight * FocusDistance * V;
		}

		Ray<float> GetRay(float S, float T, Rng32& PRNG)
		{
			vec3 Rd = LensRadius * RandomInUnitDisk(PRNG);
			vec3 Offset = U * Rd.x + V * Rd.y;

			return Ray<float>(Origin + Offset, Normalize(TL + S * Horizontal - T * Vertical - Origin - Offset));
		}
	};

}