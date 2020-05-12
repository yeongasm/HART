#pragma once
#include <Shapes/Sphere.h>
#include "Texture.h"

namespace hart
{

	struct BaseMaterial
	{
		BaseTexture* Texture;

		BaseMaterial(BaseTexture* Texture) :
			Texture(Texture) {}

		~BaseMaterial()
		{
			// TODO:
			// Let the resource manager delete the texture from the heap.
			delete Texture;
		}

		virtual bool Scatter(const Ray<float>& RayIn, const HitRecord& Record, vec3& Attenuation, Ray<float>& ScatteredRay, Rng32& PRNG) const = 0;

		virtual rgb Emitted(float U, float V, const HitRecord& Record) const
		{
			return rgb(0.0f);
		}

	};


	struct LambertianDiffused : public BaseMaterial
	{
		LambertianDiffused(BaseTexture* Texture) :
			BaseMaterial(Texture) {}

		bool Scatter(const Ray<float>& RayIn, const HitRecord& Record, vec3& Attenuation, Ray<float>& ScatteredRay, Rng32& PRNG) const override
		{
			vec3 Target = Record.Point + Record.Normal + RandomPointInUnitSphere(PRNG);
			
			ScatteredRay = Ray<float>(Record.Point, Normalize(Target - Record.Point));

			// Add an epsilon to the scattered ray's origin to avoid self-intersection where
			// the new ray immediately intersects the same surface again.
			ScatteredRay.Origin += Record.Normal * 0.005f;
			Attenuation = Texture->Value(Record.U, Record.V, Record.Point);

			return true;
		}
	};

	
	struct Metal : public BaseMaterial
	{
	public:

		Metal(BaseTexture* Texture, float RoughnessFactor) :
			BaseMaterial(Texture), Roughness(1.0f)
		{
			if (RoughnessFactor < 1.0f)
			{
				Roughness = RoughnessFactor;
			}
		}

		bool Scatter(const Ray<float>& RayIn, const HitRecord& Record, vec3& Attenuation, Ray<float>& ScatteredRay, Rng32& PRNG) const override
		{
			vec3 Reflected = Reflect(RayIn.Direction, Record.Normal);

			ScatteredRay = Ray<float>(Record.Point, Normalize(Reflected + Roughness * RandomPointInUnitSphere(PRNG)));

			// Add an epsilon to the scattered ray's origin to avoid self-intersection where
			// the new ray immediately intersects the same surface again.
			ScatteredRay.Origin += Record.Normal * 0.005f;
			Attenuation = Texture->Value(Record.U, Record.V, Record.Point);

			return Dot(ScatteredRay.Direction, Record.Normal) > 0;
		}

	private:

		float Roughness;

	};


	struct Dielectric : public BaseMaterial
	{
	private:

		float RefractiveIndex;

		float Schlick(const vec3& Incident, const vec3& Normal, float N1, float N2) const
		{
			float R0 = (N1 - N2) / (N1 + N2);
			R0 *= R0;

			float CosX = -Dot(Normal, Incident);

			if (N1 > N2)
			{
				const float N = N1 / N2;
				const float SinT2 = N * N * (1.0f - CosX * CosX);

				if (SinT2 > 1.0f)
				{
					return 1.0f;
				}

				CosX = (float)sqrt(1.0f - SinT2);
			}

			const float X = 1.0f - CosX;

			return R0 + (1.0f - R0) * X * X * X * X * X;
		}

	public:

		Dielectric(BaseTexture* Texture, float Ri) :
			BaseMaterial(Texture), RefractiveIndex(Ri) {}

		bool Scatter(const Ray<float>& RayIn, const HitRecord& Record, vec3& Attenuation, Ray<float>& ScatteredRay, Rng32& PRNG) const override
		{
			vec3 Reflected = Reflect(RayIn.Direction, Record.Normal);
			vec3 Refracted = vec3(0.0f);

			float ReflectProb = 0.0f;

			// We always assume that the ray shoots into the object.
			float N1 = 1.0f;
			float N2 = RefractiveIndex;
			vec3 OutwardNormal = Record.Normal;

			Attenuation = Texture->Value(Record.U, Record.V, Record.Point);

			// When ray shoots through object and back into vacuum.
			if (Dot(RayIn.Direction, Record.Normal) > 0.0f)
			{
				OutwardNormal = -Record.Normal;
				N1 = RefractiveIndex;
				N2 = 1.0f;
			}

			ReflectProb = 1.0f;

			if (Refract(RayIn.Direction, OutwardNormal, N1, N2, Refracted))
			{
				ReflectProb = Schlick(RayIn.Direction, OutwardNormal, N1, N2);
			}
						
			if ((UnitRand<float>(PRNG)) < ReflectProb)
			{
				ScatteredRay = Ray<float>(Record.Point, Normalize(Reflected));
			}
			else
			{
				ScatteredRay = Ray<float>(Record.Point, Normalize(Refracted));
			}

			// Reducing the size of the epsilon removed the black dot bug.
			// Not sure why, need to research into it.
			ScatteredRay.Origin += -OutwardNormal * 0.01f;

			return true;
		}
	};

	
	struct DiffusedLight : public BaseMaterial
	{

		DiffusedLight(BaseTexture* Texture, float I) :
			BaseMaterial(Texture), Intensity(I) {}

		bool Scatter(const Ray<float>& RayIn, const HitRecord& Record, vec3& Attenuation, Ray<float>& ScatteredRay, Rng32& PRNG) const override
		{
			return false;
		}

		rgb Emitted(float U, float V, const HitRecord& Record) const override
		{
			return Texture->Value(U, V, Record.Point) * Intensity;
		}

		float Intensity;
	};


	struct SunLight : public BaseMaterial
	{
		SunLight(BaseTexture* Texture, vec3 Dir, float I) :
			BaseMaterial(Texture), Direction(Dir), Intensity(I) {}

		bool Scatter(const Ray<float>& RayIn, const HitRecord& Record, vec3& Attenuation, Ray<float>& ScatteredRay, Rng32& PRNG) const override
		{
			return false;
		}

		rgb Emitted(float U, float V, const HitRecord& Record) const override
		{
			float Radiance = Gfl::Max(Dot(Normalize(Direction), Normalize(Record.Normal)), 1.0f);

			return Texture->Value(U, V, Record.Point) * Radiance * Intensity;
		}

		vec3 Direction;
		float Intensity;
	};

}