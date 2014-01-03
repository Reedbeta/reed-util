#include "util.h"

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <limits>

namespace util
{
	extern const float infty = std::numeric_limits<float>::infinity();
	extern const float NaN = std::numeric_limits<float>::quiet_NaN();

	bool isfinite(float f)
	{
		union { uint i; float f; } u;
		u.f = f;
		return ((u.i & 0x7f800000) != 0x7f800000);
	}

	void __declspec(noreturn) exit(const char * fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		::exit(1);
	}



	// RNG implementation

	uint wangHash(uint x)
	{
		// Thomas Wang's integer hash, as reported by Bob Jenkins
		x = (x ^ 61) ^ (x >> 16);
		x *= 9;
		x = x ^ (x >> 4);
		x *= 0x27d4eb2d;
		x = x ^ (x >> 15);
		return x;
	}

	void RNG::seed(uint seed)
	{
		m_state = wangHash(seed);
	}

	void RNG::seed()
	{
		seed(uint(time(nullptr)));
	}

	uint RNG::randUint()
	{
		// Xorshift algorithm from George Marsaglia's paper
		m_state ^= (m_state << 13);
		m_state ^= (m_state >> 17);
		m_state ^= (m_state << 5);
		return m_state;
	}

	int RNG::randInt(int min, int max)
	{
		// Note: this doesn't give a uniform distribution if |max - min|
		// isn't a divisor of 2^32.  Problems will be more apparent the
		// larger |max - min| is.  Could use rejection sampling to fix.
		return min + (randUint() % (max - min));
	}

	float RNG::randFloat()
	{
		return float(randUint()) * (1.0f / 4294967296.0f);
	}

	float RNG::randFloat(float min, float max)
	{
		return lerp(min, max, randFloat());
	}



	// Projection matrix implementations

	float4x4 orthoProjD3DStyle(float left, float right, float bottom, float top, float near, float far)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (far - near);
		return makefloat4x4(
					2.0f * xScale, 0, 0, 0,
					0, 2.0f * yScale, 0, 0,
					0, 0, -zScale, 0,
					-(left + right) * xScale, -(bottom + top) * yScale, -near * zScale, 1);
	}

	float4x4 orthoProjOGLStyle(float left, float right, float bottom, float top, float near, float far)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (far - near);
		return makefloat4x4(
					2.0f * xScale, 0, 0, 0,
					0, 2.0f * yScale, 0, 0,
					0, 0, -2.0f * zScale, 0,
					-(left + right) * xScale, -(bottom + top) * yScale, -(near + far) * zScale, 1);
	}

	float4x4 perspProjD3DStyle(float left, float right, float bottom, float top, float near, float far)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (far - near);
		return makefloat4x4(
					2.0f * near * xScale, 0, 0, 0,
					0, 2.0f * near * yScale, 0, 0,
					(left + right) * xScale, (bottom + top) * yScale, -far * zScale, -1,
					0, 0, -near * far * zScale, 0);
	}

	float4x4 perspProjOGLStyle(float left, float right, float bottom, float top, float near, float far)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (far - near);
		return makefloat4x4(
					2.0f * near * xScale, 0, 0, 0,
					0, 2.0f * near * yScale, 0, 0,
					(left + right) * xScale, (bottom + top) * yScale, -(near + far) * zScale, -1,
					0, 0, -2.0f * near * far * zScale, 0);
	}

	float4x4 perspProjD3DStyle(float verticalFOV, float aspect, float near, float far)
	{
		float yScale = 1.0f / tanf(0.5f * verticalFOV);
		float xScale = yScale / aspect;
		float zScale = 1.0f / (far - near);
		return makefloat4x4(
					xScale, 0, 0, 0,
					0, yScale, 0, 0,
					0, 0, -far * zScale, -1,
					0, 0, -near * far * zScale, 0);
	}

	float4x4 perspProjOGLStyle(float verticalFOV, float aspect, float near, float far)
	{
		float yScale = 1.0f / tanf(0.5f * verticalFOV);
		float xScale = yScale / aspect;
		float zScale = 1.0f / (far - near);
		return makefloat4x4(
					xScale, 0, 0, 0,
					0, yScale, 0, 0,
					0, 0, -(near + far) * zScale, -1,
					0, 0, -2.0f * near * far * zScale, 0);
	}



	// Affine transformation implementations

	affine2 rotation(float radians)
	{
		float sinTheta = sinf(radians);
		float cosTheta = cosf(radians);
		return makeaffine2(
					cosTheta, sinTheta,
					-sinTheta, cosTheta,
					0, 0);
	}

	affine3 rotation(float3_arg axis, float radians)
	{
		// Note: assumes axis is normalized
		float sinTheta = sinf(radians);
		float cosTheta = cosf(radians);

		// Build matrix that does cross product by axis (on the right)
		float3x3 crossProductMat = makefloat3x3(
										0, axis.z, -axis.y,
										-axis.z, 0, axis.x,
										axis.y, -axis.x, 0);

		// Matrix form of Rodrigues' rotation formula
		float3x3 mat = diagonal<float, 3>(cosTheta) +
						crossProductMat * sinTheta +
						outerProduct(axis, axis) * (1.0f - cosTheta);

		return makeaffine3(mat, makefloat3(0.0f));
	}

	affine2 lookat(float2_arg look)
	{
		return makeaffine2(look, orthogonal(look), makefloat2(0.0f));
	}

	affine3 lookatX(float3_arg look)
	{
		float3 left = orthogonal(look);
		float3 up = cross(look, left);
		return makeaffine3(look, left, up, makefloat3(0.0f));
	}

	affine3 lookatX(float3_arg look, float3_arg up)
	{
		float3 left = cross(up, look);
		float3 trueUp = cross(look, left);
		return makeaffine3(look, left, trueUp, makefloat3(0.0f));
	}

	affine3 lookatZ(float3_arg look)
	{
		float3 left = orthogonal(look);
		float3 up = cross(look, left);
		return makeaffine3(-left, up, -look, makefloat3(0.0f));
	}

	affine3 lookatZ(float3_arg look, float3_arg up)
	{
		float3 left = cross(up, look);
		float3 trueUp = cross(look, left);
		return makeaffine3(-left, trueUp, -look, makefloat3(0.0f));
	}



	// Convert memory layouts to and from SIMD-friendly (AOSOA) layout

	void convertToSIMD(
			uint numComponents,
			uint numVectors,
			const void * pInput,
			uint inputStrideBytes,
			void * pOutput,
			uint outputStrideBytes)
	{
		static const int simdWidth = 4;

		assert(numComponents > 0);
		assert(pInput);
		assert(inputStrideBytes >= sizeof(float) * numComponents);
		assert(pOutput);
		assert((size_t)pOutput % (simdWidth * sizeof(float)) == 0);
		assert(outputStrideBytes >= simdWidth * sizeof(float) * numComponents);

		// Do the part that's a multiple of simdWidth
		for (; numVectors >= simdWidth; numVectors -= simdWidth)
		{
			for (uint i = 0; i < simdWidth; ++i)
			{
				for (uint j = 0; j < numComponents; ++j)
					((float *)pOutput)[simdWidth*j + i] = ((float *)pInput)[j];

				pInput = advanceBytes(pInput, inputStrideBytes);
			}

			pOutput = advanceBytes(pOutput, outputStrideBytes);
		}

		// Do any part left over
		for (uint i = 0; i < numVectors; ++i)
		{
			for (uint j = 0; j < numComponents; ++j)
				((float *)pOutput)[simdWidth*j + i] = ((float *)pInput)[j];

			pInput = advanceBytes(pInput, inputStrideBytes);
		}
	}

	void convertFromSIMD(
			uint numComponents,
			uint numVectors,
			const void * pInput,
			uint inputStrideBytes,
			void * pOutput,
			uint outputStrideBytes)
	{
		static const int simdWidth = 4;

		assert(numComponents > 0);
		assert(pInput);
		assert((size_t)pInput % (simdWidth * sizeof(float)) == 0);
		assert(inputStrideBytes >= simdWidth * sizeof(float) * numComponents);
		assert(pOutput);
		assert(outputStrideBytes >= sizeof(float) * numComponents);

		// Do the part that's a multiple of simdWidth
		for (; numVectors >= simdWidth; numVectors -= simdWidth)
		{
			for (uint i = 0; i < simdWidth; ++i)
			{
				for (uint j = 0; j < numComponents; ++j)
					((float *)pOutput)[j] = ((float *)pInput)[simdWidth*j + i];

				pOutput = advanceBytes(pOutput, outputStrideBytes);
			}

			pInput = advanceBytes(pInput, inputStrideBytes);
		}

		// Do any part left over
		for (uint i = 0; i < numVectors; ++i)
		{
			for (uint j = 0; j < numComponents; ++j)
				((float *)pOutput)[j] = ((float *)pInput)[simdWidth*j + i];

			pOutput = advanceBytes(pOutput, outputStrideBytes);
		}
	}
}



// Vector/matrix unit testing crap
// !!!UNDONE: move somewhere else?

#include <vector>

void passPointer(const float * p)
{
	(void)p;
}

void testVectors()
{
	using namespace util;

	vector<float, 5> foo5 = { 1, 2, 3, 4, 5 };
	foo5.m_data[4];
	foo5[4];
	passPointer(foo5);
	foo5 + foo5;
	foo5 + 47.0f;
	47.0f + foo5;
	foo5 - foo5;
	foo5 - 47.0f;
	47.0f - foo5;
	-foo5;
	foo5 * foo5;
	foo5 * 47.0f;
	47.0f * foo5;
	foo5 / foo5;
	foo5 / 47.0f;
	47.0f / foo5;
	foo5 += foo5;
	foo5 += 47.0f;
	foo5 -= foo5;
	foo5 -= 47.0f;
	foo5 *= foo5;
	foo5 *= 47.0f;
	foo5 /= foo5;
	foo5 /= 47.0f;
	foo5 == foo5;
	foo5 == 47.0f;
	foo5 != foo5;
	foo5 != 47.0f;
	foo5 < foo5;
	foo5 < 47.0f;
	foo5 > foo5;
	foo5 > 47.0f;
	foo5 <= foo5;
	foo5 <= 47.0f;
	foo5 >= foo5;
	foo5 >= 47.0f;
	dot(foo5, foo5);
	lengthSquared(foo5);
	length(foo5);
	foo5 = normalize(foo5);
	isfinite(foo5);
	any(foo5 < 47.0f);
	all(foo5 > 47.0f);
	select(foo5 < 47.0f, foo5, foo5);
	foo5 = min(foo5, foo5);
	foo5 = max(foo5, foo5);
	foo5 = abs(foo5);
	foo5 = clamp(foo5, foo5, foo5);
	foo5 = saturate(foo5);
	minComponent(foo5);
	maxComponent(foo5);
	foo5 = lerp(foo5, foo5, 0.5f);
	foo5 = square(foo5);

	vector<int, 5> ifoo5 = { 1, 2, 3, 4, 5 };
	ifoo5 & ifoo5;
	ifoo5 | ifoo5;
	ifoo5 ^ ifoo5;
	!ifoo5;
	~ifoo5;
	ifoo5 &= ifoo5;
	ifoo5 |= ifoo5;
	ifoo5 ^= ifoo5;

	float4 foo4 = { 1, 2, 3, 4};
	foo4.w;
	foo4.a;
	foo4[3];
	passPointer(foo4);

	float3 foo3 = { 1, 2, 3 };
	foo3.z;
	foo3.b;
	foo3[2];
	passPointer(foo3);
	cross(foo3, foo3);
	orthogonal(foo3);
	foo3 = foo4.xyz;

	float2 foo2 = { 1, 2 };
	foo2.y;
	foo2.v;
	foo2[1];
	passPointer(foo2);
	orthogonal(foo2);
	foo2 = foo3.xy;

	float bazArray[] = { 1, 2, 3, 4 };
	makefloat4(bazArray);

	__m128 simdA = {}, simdB = {};
	simdA + simdB;
	simdA += simdB;
	vector<__m128, 4> simdVector;
	simdVector + simdVector;
	simdVector + _mm_set_ps(47, 47, 47, 47);
}



void testMatrices()
{
	using namespace util;

	matrix<float, 5, 5> foo5 = { 1, 2, 3, 4, 5 };
	vector<float, 5> bar5;
	foo5.m_data[4];
	foo5[4][3] = 47.0f;
	passPointer(foo5);
	foo5 + foo5;
	foo5 + 47.0f;
	47.0f + foo5;
	foo5 - foo5;
	foo5 - 47.0f;
	47.0f - foo5;
	-foo5;
	foo5 * 47.0f;
	47.0f * foo5;
	foo5 / 47.0f;
	47.0f / foo5;
	foo5 += foo5;
	foo5 += 47.0f;
	foo5 -= foo5;
	foo5 -= 47.0f;
	foo5 *= 47.0f;
	foo5 /= 47.0f;
	foo5 == foo5;
	foo5 == 47.0f;
	foo5 != foo5;
	foo5 != 47.0f;
	foo5 < foo5;
	foo5 < 47.0f;
	foo5 > foo5;
	foo5 > 47.0f;
	foo5 <= foo5;
	foo5 <= 47.0f;
	foo5 >= foo5;
	foo5 >= 47.0f;
	foo5 * foo5;
	foo5 *= foo5;
	foo5 * bar5;
	bar5 * foo5;
	bar5 *= foo5;
	foo5 = transpose(foo5);
	foo5 = pow(foo5, 5);
	foo5 = inverse(foo5);
	determinant(foo5);
	trace(foo5);
	foo5 = diagonal<float, 5>(47.0f);
	foo5 = diagonal(bar5);
	foo5 = outerProduct(bar5, bar5);
	isfinite(foo5);
	any(foo5 < 47.0f);
	all(foo5 > 47.0f);
	select(foo5 < 47.0f, foo5, foo5);
	foo5 = min(foo5, foo5);
	foo5 = max(foo5, foo5);
	foo5 = abs(foo5);
	foo5 = clamp(foo5, foo5, foo5);
	foo5 = saturate(foo5);
	minComponent(foo5);
	maxComponent(foo5);
	foo5 = lerp(foo5, foo5, 0.5f);
	foo5 = square(foo5);

	matrix<int, 5, 5> ifoo5 = { 1, 2, 3, 4, 5 };
	ifoo5 & ifoo5;
	ifoo5 | ifoo5;
	ifoo5 ^ ifoo5;
	!ifoo5;
	~ifoo5;
	ifoo5 &= ifoo5;
	ifoo5 |= ifoo5;
	ifoo5 ^= ifoo5;

	float bazArray[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	makefloat4x4(bazArray);

	matrix<float, 4, 3> foo4x3;
	float4 bar4;
	float3 bar3;
	bar4 = foo4x3 * bar3;
	bar3 = bar4 * foo4x3;
	matrix<float, 3, 4> foo3x4 = transpose(foo4x3);
	(void) foo3x4;
	foo4x3 = outerProduct(bar4, bar3);
}



void testAffine()
{
	using namespace util;

	point<float, 5> foo5 = { 1, 2, 3, 4, 5 };
	vector<float, 5> bar5 = { 6, 7, 8, 9, 10 };
	foo5.m_data[4];
	foo5[4];
	passPointer(foo5);
	foo5 + bar5;
	foo5 - bar5;
	foo5 - foo5;
	foo5 += bar5;
	foo5 -= bar5;
	foo5 == foo5;
	foo5 == 47.0f;
	foo5 != foo5;
	foo5 != 47.0f;
	foo5 < foo5;
	foo5 < 47.0f;
	foo5 > foo5;
	foo5 > 47.0f;
	foo5 <= foo5;
	foo5 <= 47.0f;
	foo5 >= foo5;
	foo5 >= 47.0f;
	dot(foo5, bar5);
	dot(bar5, foo5);
	isfinite(foo5);
	any(foo5 < 47.0f);
	all(foo5 > 47.0f);
	select(foo5 < 47.0f, foo5, foo5);
	min(foo5, foo5);
	max(foo5, foo5);
	minComponent(foo5);
	maxComponent(foo5);
	lerp(foo5, foo5, 0.5f);

	point3 foo3 = { 1, 2, 3 };
	foo3.z;
	foo3.b;
	foo3[2];
	passPointer(foo3);

	point2 foo2 = { 1, 2 };
	foo2.y;
	foo2.v;
	foo2[1];
	passPointer(foo2);
	foo2 = foo3.xy;

	float bazArray[] = { 1, 2, 3 };
	makepoint3(bazArray);

	affine<float, 5> aff5 = { 1, 2, 3, 4, 5 };
	aff5 == aff5;
	aff5 != aff5;
	aff5 * aff5;
	aff5 *= aff5;
	foo5 * aff5;
	bar5 * aff5;
	foo5 *= aff5;
	bar5 *= aff5;
	aff5 = transpose(aff5);
	aff5 = pow(aff5, 5);
	aff5 = inverse(aff5);
	isfinite(aff5);
	aff5 = square(aff5);
	aff5 = translation(bar5);
	aff5 = scaling<float, 5>(47.0f);
	aff5 = scaling(bar5);
}
