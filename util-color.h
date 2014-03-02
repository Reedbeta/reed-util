#pragma once

namespace util
{
	// Create typedefs of float vectors to represent common color spaces.
	// These are intentionally *not* type-safe, to reduce friction.
	// Note: alpha is always linear, even in sRGB spaces.

#define DEFINE_COLOR_SPACE(name) \
			typedef float3 name; \
			typedef float3_arg name##_arg; \
			typedef float4 name##a; \
			typedef float4_arg name##a_arg; \
			name make##name(float x, float y, float z) \
				{ name v = { x, y, z }; return v; } \
			name make##name(float a) \
				{ name v = { a, a, a }; return v; } \
			name make##name(const float * a) \
				{ name v = { a[0], a[1], a[2] }; return v; } \
			name##a make##name##a(float x, float y, float z, float w) \
				{ name##a v = { x, y, z, w }; return v; } \
			name##a make##name##a(name##_arg xyz, float w) \
				{ name##a v = { xyz.x, xyz.y, xyz.z, w }; return v; } \
			name##a make##name##a(float a) \
				{ name##a v = { a, a, a, a }; return v; } \
			name##a make##name##a(const float * a) \
				{ name##a v = { a[0], a[1], a[2], a[3] }; return v; }

	DEFINE_COLOR_SPACE(rgb);		// Linear RGB space
	DEFINE_COLOR_SPACE(srgb);		// sRGB space
	DEFINE_COLOR_SPACE(hsv);		// HSV based on linear RGB space
	DEFINE_COLOR_SPACE(shsv);		// HSV based on sRGB space
	DEFINE_COLOR_SPACE(ycocg);		// YCoCg based on linear RGB space
	DEFINE_COLOR_SPACE(sycocg);		// YCoCg based on sRGB space
	DEFINE_COLOR_SPACE(cielab);		// CIELAB space

#undef DEFINE_COLOR_SPACE



	// Rec. 709 luma coefficients for linear RGB space
	static const rgb lumaCoefficients = { 0.2126f, 0.7152f, 0.0722f };
	inline float luminance(rgb_arg c)
		{ return dot(c, lumaCoefficients); }
	inline float luminance(rgba_arg c)
		{ return dot(c.rgb, lumaCoefficients); }

	// Composition operator for linear RGB space (premultiplied alpha)
	inline rgba over_premul(rgba_arg a, rgba_arg b)
		{ return makergba(a.rgb + (1.0f-a.a) * b.rgb, 1.0f - ((1.0f-a.a) * (1.0f-b.a))); }

	// Composition operator for linear RGB space (non-premultiplied alpha)
	inline rgba over_nonpremul(rgba_arg a, rgba_arg b)
		{ return makergba(lerp(b.rgb, a.rgb, a.a), 1.0f - (1.0f-a.a) * (1.0f-b.a)); }
	inline rgb over_nonpremul(rgba_arg a, rgb_arg b)
		{ return lerp(b, a.rgb, a.a); }



	// SRGB/linear color space conversions
	inline float toLinear(float c)
		{ return (c <= 0.04045f) ? c/12.92f : ::pow((c + 0.055f)/1.055f, 2.4f); }
	inline float toSRGB(float c)
		{ return (c <= 0.0031308f) ? c*12.92f : 1.055f*::pow(c, 1.0f/2.4f) - 0.055f; }
	inline rgb toLinear(srgb_arg c)
		{ return select(c <= 0.04045f, c/12.92f, pow((c + 0.055f)/1.055f, 2.4f)); }
	inline srgb toSRGB(rgb_arg c)
		{ return select(c <= 0.0031308f, c*12.92f, 1.055f*pow(c, 1.0f/2.4f) - 0.055f); }
	inline rgba toLinear(srgba_arg c)
		{ return makergba(toLinear(c.rgb), c.a); }
	inline srgba toSRGB(rgba_arg c)
		{ return makergba(toSRGB(c.rgb), c.a); }

	// RGB/HSV conversions
	hsv RGBtoHSV(rgb_arg c);
	rgb HSVtoRGB(hsv_arg c);
	inline hsva RGBtoHSV(rgba_arg c)
		{ return makehsva(RGBtoHSV(c.rgb), c.a); }
	inline rgba HSVtoRGB(hsva_arg c)
		{ return makergba(HSVtoRGB(c.xyz), c.a); }

	// RGB/YCoCg conversions
	inline ycocg RGBtoYCoCg(rgb_arg c)
		{ return makeycocg(0.25f*(c.r+2.0f*c.g+c.b), c.r-c.b, c.g - 0.5f*(c.r+c.b)); }
	inline rgb YCoCgtoRGB(ycocg_arg c)
		{ return makergb(c.x+0.5f*(c.y-c.z), c.x+0.5f*c.z, c.x-0.5f*(c.y+c.z)); }
	inline ycocga RGBtoYCoCg(rgba_arg c)
		{ return makeycocga(RGBtoYCoCg(c.rgb), c.a); }
	inline rgba YCoCgtoRGB(ycocga_arg c)
		{ return makergba(YCoCgtoRGB(c.xyz), c.a); }

	// RGB/CIELAB conversions
	cielab RGBtoCIELAB(rgb_arg c);
	rgb CIELABtoRGB(cielab_arg c);
	inline cielaba RGBtoCIELAB(rgba_arg c)
		{ return makecielaba(RGBtoCIELAB(c.rgb), c.a); }
	inline rgba CIELABtoRGB(cielaba_arg c)
		{ return makergba(CIELABtoRGB(c.xyz), c.a); }
}
