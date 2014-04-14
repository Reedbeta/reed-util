#pragma once
#include <cmath>
#include <float.h>

// Compile-time array size
template <typename T, int N> char(&dim_helper(T(&)[N]))[N];
#define dim(x) (sizeof(dim_helper(x)))
#define dim_field(S, m) dim(((S*)0)->m)
#define sizeof_field(S, m) (sizeof(((S*)0)->m))

// Compile-time assert
#define cassert(x) static_assert(x, #x)

namespace util
{
	// "uint" is a lot shorter than "unsigned int"
	typedef unsigned int uint;

	// "byte" is a lot shorter than "unsigned char"
	typedef unsigned char byte;

	// More short names for integer types
	typedef char				i8;
	typedef	short				i16;
	typedef int					i32;
	typedef __int64				i64;
	typedef unsigned char		u8;
	typedef unsigned short		u16;
	typedef unsigned int		u32;
	typedef unsigned __int64	u64;

	// Delicious pi
	static const float pi = 3.141592654f;

	// Convenient float constants
	static const float epsilon = 1e-6f;		// A reasonable general-purpose epsilon
	extern const float infinity;
	extern const float NaN;

	// Generic swap
	template <typename T>
	void swap(T & a, T & b) { T c = a; a = b; b = c; }

	// Generic min/max/abs/clamp/saturate
	template <typename T>
	T min(T a, T b) { return (a < b) ? a : b; }
	template <typename T>
	T max(T a, T b) { return (a < b) ? b : a; }
	template <typename T>
	T abs(T a) { return (a < T(0)) ? -a : a; }
	template <typename T>
	T clamp(T value, T lower, T upper) { return min(max(value, lower), upper); }
	template <typename T>
	T saturate(T value) { return clamp(value, T(0), T(1)); }

	// Generic lerp
	template <typename T>
	T lerp(T a, T b, float u) { return a + (b - a) * u; }

	// Generic square
	template <typename T>
	T square(T a) { return a*a; }

	// Equality test with epsilon
	inline bool isnear(float a, float b, float epsilon = util::epsilon)
		{ return (abs(b - a) < epsilon); }

	// Test for finiteness
	inline bool isfinite(float f)
	{
		union { uint i; float f; } u;
		u.f = f;
		return ((u.i & 0x7f800000) != 0x7f800000);
	}

	// Rounding to nearest integer
	inline int round(float f)
		{ return int(floor(f + 0.5f)); }

	// Modulus with always positive remainders (assuming positive divisor)
	inline int modPositive(int dividend, int divisor)
	{
		int result = dividend % divisor;
		if (result < 0)
			result += divisor;
		return result;
	}
	inline float modPositive(float dividend, float divisor)
	{
		float result = fmod(dividend, divisor);
		if (result < 0)
			result += divisor;
		return result;
	}

	// Print an error message and immediately exit with code 1
	void __declspec(noreturn) exit(const char * fmt, ...);

	// Advance a pointer by a given number of bytes, regardless of pointer's type
	// (note: number of bytes can be negative)
	template <typename T>
	inline T * advanceBytes(T * ptr, int bytes)
		{ return (T *)((byte *)ptr + bytes); }
}

// Logging and errors
#include "util-log.h"

// Random number library
#include "util-rng.h"

// Math libraries
#include "util-vector.h"
#include "util-matrix.h"
#include "util-affine.h"
#include "util-simd.h"
#include "util-box.h"
#include "util-color.h"
#include "util-quat.h"
#include "half/half.h"
