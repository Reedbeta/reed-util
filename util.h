#pragma once
#include <cmath>
#include <cstdarg>
#include <float.h>
#include <vector>

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
	inline bool isnear(float a, float b, float eps = util::epsilon)
		{ return (abs(b - a) < eps); }

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

	// Base-2 exp and log
	inline float exp2f(float x) { return expf(0.693147181f * x); }
	inline float log2f(float x) { return 1.442695041f * logf(x); }

	// Integer log2, with rounding up or down
	inline int log2_floor(int x) { if (x <= 0) return 0; unsigned long c = 0; _BitScanReverse(&c, x); return int(c); }
	inline int log2_ceil(int x) { return (x > 0) ? (log2_floor(x - 1) + 1) : 0; }

	// Round up or down to nearest power of 2
	inline bool ispow2(int x) { return (x > 0) && ((x & (x - 1)) == 0); }
	inline int pow2_floor(int x) { return (1 << log2_floor(x)); }
	inline int pow2_ceil(int x) { return (1 << log2_ceil(x)); }

	// Integer division, with rounding up (assuming positive arguments)
	inline int div_ceil(int dividend, int divisor) { return (dividend + (divisor - 1)) / divisor; }

	// Integer rounding to multiples
	inline int roundDown(int i, int multiple) { return (i / multiple) * multiple; }
	inline int roundUp(int i, int multiple) { return ((i + (multiple - 1)) / multiple) * multiple; }

	// Advance a pointer by a given number of bytes, regardless of pointer's type
	// (note: number of bytes can be negative)
	template <typename T>
	inline T * advanceBytes(T * ptr, int bytes)
		{ return (T *)((byte *)ptr + bytes); }

	// Print an error message to stderr and immediately exit with code 1
	void __declspec(noreturn) exit(const char * fmt, ...);

	// Load an entire file into memory
	bool LoadFile(const char * path, std::vector<byte> * pDataOut, bool text = false);

	// In-place destructive string tokenizer - like strtok, but safer (stateless)
	// Returns pointer to the start of the next token, and updates str to point to the
	// remainder of the string
	char * tokenize(char * & str, const char * delim);
}

// Logging and errors
#include "util-log.h"
#include "util-err.h"
