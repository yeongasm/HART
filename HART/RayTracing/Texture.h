#pragma once
#include <Math/Vector.h>


namespace hart
{

	struct BaseTexture
	{
		BaseTexture(rgb Colour) :
			Albedo(Colour) {}

		~BaseTexture() {}

		virtual rgb Value(float U, float V, const vec3& P) const
		{
			return Albedo;
		}

		rgb Albedo;
	};


	struct GradientTexture : public BaseTexture
	{
		GradientTexture(size_t Width, size_t Height) :
			BaseTexture(rgb(1.0f)), Width(Width), Height(Height) {}

		~GradientTexture() {}

		virtual rgb Value(float U, float V, const vec3& P) const
		{
			const vec3 RGBLeft	= vec3(0.0f, 0.0f, 1.0f);
			const vec3 RGBLM	= vec3(0.0f, 1.0f, 1.0f);
			const vec3 RGBMid	= vec3(0.0f, 1.0f, 0.0f);
			const vec3 RGBMR	= vec3(1.0f, 1.0f, 0.0f);
			const vec3 RGBRight = vec3(1.0f, 0.0f, 0.0f);

			const vec3 Black = vec3(0.0f);

			float h  = 1.0f / 4.0f;
			float h2 = 2.0f / 4.0f;
			float h3 = 3.0f / 4.0f;

			const float x = U;

			vec3 col1	= x < h ? Mix(RGBLeft, RGBLM, x / h) : Black;

			vec3 c2b	= Mix(RGBLM, RGBMid,	(x - h) / h);
			vec3 col2	= h < x ? x < h2 ? c2b : Black : Black;

			vec3 c3b	= Mix(RGBMid, RGBMR,	(x - h2) / h);
			vec3 col3	= h2 < x ? x < h3 ? c3b : Black : Black;

			vec3 c4b	= Mix(RGBMR, RGBRight,	(x - h3) / h);
			vec3 col4	= h3 < x ? x < 1.0f ? c4b : Black : Black;

			vec3 out	= col1 + col2 + col3 + col4;

			return out;
		}

		size_t Width, Height;
	};


}