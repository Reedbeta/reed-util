#pragma once

namespace util
{
	// Create typedefs of float vectors to represent RGB/RGBA values.
	// These are intentionally *not* type-safe, to reduce friction.
	typedef float3 rgb;
	typedef float4 rgba;

	// Typedefs for gamma-encoded color values, using the sRGB curve.
	// Note: alpha is still linear.
	typedef float3 srgb;
	typedef float4 srgba;



	// Rec. 709 luma coefficients for linear RGB space
	static const float lumaCoefficients[] = { 0.2126f, 0.7152f, 0.0722f };
	inline float luminance(rgb c)
		{ return dot(c, float3(lumaCoefficients)); }
	inline float luminance(rgba c)
		{ return dot(c.rgb, float3(lumaCoefficients)); }

	// Composition operator for linear RGB space (premultiplied alpha)
	inline rgb blendOver(rgba a, rgb b)
		{ return a.rgb + (1.0f - a.a) * b; }
	inline rgba blendOver(rgba a, rgba b)
		{ return rgba(a.rgb + (1.0f - a.a) * b.rgb, a.a + b.a - a.a * b.a); }

	// Convert between non-premultiplied and premultipled alpha
	inline rgba premultiplyAlpha(rgba a)
		{ return rgba(a.rgb * a.a, a.a); }
	inline rgba unPremultiplyAlpha(rgba a)
		{ return rgba(a.rgb / a.a, a.a); }



	// SRGB/linear color space conversions
	inline float SRGBtoLinear(float c)
		{ return (c <= 0.04045f) ? c / 12.92f : ::pow((c + 0.055f) / 1.055f, 2.4f); }
	inline float linearToSRGB(float c)
		{ return (c <= 0.0031308f) ? c * 12.92f : 1.055f*::pow(c, 1.0f/2.4f) - 0.055f; }
	inline rgb SRGBtoLinear(srgb c)
		{ return select(c <= 0.04045f, c / 12.92f, pow((c + 0.055f) / 1.055f, 2.4f)); }
	inline srgb linearToSRGB(rgb c)
		{ return select(c <= 0.0031308f, c * 12.92f, 1.055f*pow(c, 1.0f/2.4f) - 0.055f); }
	inline rgba SRGBtoLinear(srgba c)
		{ return rgba(SRGBtoLinear(c.rgb), c.a); }
	inline srgba linearToSRGB(rgba c)
		{ return rgba(linearToSRGB(c.rgb), c.a); }

	// RGB/HSV conversions
	float3 RGBtoHSV(rgb c);
	rgb HSVtoRGB(float3 c);
	inline float4 RGBAtoHSVA(rgba c)
		{ return float4(RGBtoHSV(c.rgb), c.a); }
	inline rgba HSVAtoRGBA(float4 c)
		{ return rgba(HSVtoRGB(c.xyz), c.a); }

	// RGB/YCoCg conversions
	inline float3 RGBtoYCoCg(rgb c)
		{ return { 0.25f * (c.r + 2.0f * c.g + c.b), c.r - c.b, c.g - 0.5f * (c.r + c.b) }; }
	inline rgb YCoCgtoRGB(float3 c)
		{ return { c.x + 0.5f * (c.y - c.z), c.x + 0.5f * c.z, c.x - 0.5f * (c.y + c.z) }; }
	inline float4 RGBAtoYCoCgA(rgba c)
		{ return float4(RGBtoYCoCg(c.rgb), c.a); }
	inline rgba YCoCgAtoRGBA(float4 c)
		{ return rgba(YCoCgtoRGB(c.xyz), c.a); }

	// RGB/CIELAB conversions
	float3 RGBtoCIELAB(rgb c);
	rgb CIELABtoRGB(float3 c);
	inline float4 RGBAtoCIELABA(rgba c)
		{ return float4(RGBtoCIELAB(c.rgb), c.a); }
	inline rgba CIELABAtoRGBA(float4 c)
		{ return rgba(CIELABtoRGB(c.xyz), c.a); }
}
