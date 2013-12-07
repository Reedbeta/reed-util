#include "reed-util.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <limits>

namespace reed
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

	void RNG::seed(uint seed)
	{
		// Thomas Wang's integer hash, as reported by Bob Jenkins
		seed = (seed ^ 61) ^ (seed >> 16);
		seed *= 9;
		seed = seed ^ (seed >> 4);
		seed *= 0x27d4eb2d;
		seed = seed ^ (seed >> 15);
		m_state = seed;
	}

	void RNG::seedWithTime()
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
		// Note: this doesn't necessarily give a uniform distribution;
		// problems may be seen when min and max are far apart
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
	using namespace reed;

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
	normalize(foo5);
	isfinite(foo5);
	any(foo5 < 47.0f);
	all(foo5 > 47.0f);
	select(foo5 < 47.0f, foo5, foo5);
	min(foo5, foo5);
	max(foo5, foo5);
	abs(foo5);
	clamp(foo5, foo5, foo5);
	saturate(foo5);
	minComponent(foo5);
	maxComponent(foo5);

	float4 foo4 = { 1, 2, 3, 4};
	foo4.w;
	foo4.a;
	foo4[3];
	passPointer(foo4);

	float3 foo3 = { 1, 2, 3 };
	foo3.z;
	foo3.b;
	foo4[2];
	passPointer(foo3);
	cross(foo3, foo3);
	orthogonal(foo3);
	foo3 = foo4.xyz;

	float2 foo2 = { 1, 2 };
	foo2.y;
	foo2.v;
	foo4[1];
	passPointer(foo2);
	orthogonal(foo2);
	foo2 = foo3.xy;

	float bazArray[] = { 1, 2, 3, 4 };
	std::vector<float> bazVector(&bazArray[0], &bazArray[dim(bazArray)]);
	makefloat4(bazArray);
	makefloat4(bazVector);
}



void testMatrices()
{
	using namespace reed;

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
	transpose(foo5);
	pow(foo5, 5);
	inverse(foo5);
	determinant(foo5);
	trace(foo5);
	outerproduct(bar5, bar5);
	isfinite(foo5);
	any(foo5 < 47.0f);
	all(foo5 > 47.0f);
	select(foo5 < 47.0f, foo5, foo5);
	min(foo5, foo5);
	max(foo5, foo5);
	abs(foo5);
	clamp(foo5, foo5, foo5);
	saturate(foo5);
	minComponent(foo5);
	maxComponent(foo5);

	float bazArray[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	std::vector<float> bazVector(&bazArray[0], &bazArray[dim(bazArray)]);
	makefloat4x4(bazArray);
	makefloat4x4(bazVector);

	matrix<float, 4, 3> foo4x3;
	float4 bar4;
	float3 bar3;
	bar4 = foo4x3 * bar3;
	bar3 = bar4 * foo4x3;
	matrix<float, 3, 4> foo3x4 = transpose(foo4x3);
	(void) foo3x4;
	foo4x3 = outerproduct(bar4, bar3);
}
