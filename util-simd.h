#pragma once
#include <emmintrin.h>

namespace util
{
	// Overloaded operators for __m128, so it can be used in vectors

	inline __m128 operator + (__m128 a, __m128 b)
	{
		return _mm_add_ps(a, b);
	}
	inline __m128 operator + (float a, __m128 b)
	{
		return _mm_add_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator + (__m128 a, float b)
	{
		return _mm_add_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator - (__m128 a, __m128 b)
	{
		return _mm_sub_ps(a, b);
	}
	inline __m128 operator - (float a, __m128 b)
	{
		return _mm_sub_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator - (__m128 a, float b)
	{
		return _mm_sub_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator - (__m128 a)
	{
		return _mm_sub_ps(_mm_setzero_ps(), a);
	}

	inline __m128 operator * (__m128 a, __m128 b)
	{
		return _mm_mul_ps(a, b);
	}
	inline __m128 operator * (float a, __m128 b)
	{
		return _mm_mul_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator * (__m128 a, float b)
	{
		return _mm_mul_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator / (__m128 a, __m128 b)
	{
		return _mm_div_ps(a, b);
	}
	inline __m128 operator / (float a, __m128 b)
	{
		return _mm_div_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator / (__m128 a, float b)
	{
		return _mm_div_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator & (__m128 a, __m128 b)
	{
		return _mm_and_ps(a, b);
	}
	inline __m128 operator & (float a, __m128 b)
	{
		return _mm_and_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator & (__m128 a, float b)
	{
		return _mm_and_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator | (__m128 a, __m128 b)
	{
		return _mm_or_ps(a, b);
	}
	inline __m128 operator | (float a, __m128 b)
	{
		return _mm_or_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator | (__m128 a, float b)
	{
		return _mm_or_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator ^ (__m128 a, __m128 b)
	{
		return _mm_xor_ps(a, b);
	}
	inline __m128 operator ^ (float a, __m128 b)
	{
		return _mm_xor_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator ^ (__m128 a, float b)
	{
		return _mm_xor_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator ! (__m128 a)
	{
		// Note: there's no bitwise-not, so we have to use "andnot" (= ~a & b) with b = true.
		return _mm_andnot_ps(a, _mm_castsi128_ps(_mm_set_epi32(~0, ~0, ~0, ~0)));
	}

	inline __m128 operator ~ (__m128 a)
	{
		// Note: there's no bitwise-not, so we have to use "andnot" (= ~a & b) with b = true.
		return _mm_andnot_ps(a, _mm_castsi128_ps(_mm_set_epi32(~0, ~0, ~0, ~0)));
	}

	inline __m128 & operator += (__m128 & a, __m128 b)
	{
		a = a + b;
		return a;
	}
	inline __m128 & operator += (__m128 & a, float b)
	{
		a = a + b;
		return a;
	}

	inline __m128 & operator -= (__m128 & a, __m128 b)
	{
		a = a - b;
		return a;
	}
	inline __m128 & operator -= (__m128 & a, float b)
	{
		a = a - b;
		return a;
	}

	inline __m128 & operator *= (__m128 & a, __m128 b)
	{
		a = a * b;
		return a;
	}
	inline __m128 & operator *= (__m128 & a, float b)
	{
		a = a * b;
		return a;
	}

	inline __m128 & operator /= (__m128 & a, __m128 b)
	{
		a = a / b;
		return a;
	}
	inline __m128 & operator /= (__m128 & a, float b)
	{
		a = a / b;
		return a;
	}

	inline __m128 & operator &= (__m128 & a, __m128 b)
	{
		a = a & b;
		return a;
	}
	inline __m128 & operator &= (__m128 & a, float b)
	{
		a = a & b;
		return a;
	}

	inline __m128 & operator |= (__m128 & a, __m128 b)
	{
		a = a | b;
		return a;
	}
	inline __m128 & operator |= (__m128 & a, float b)
	{
		a = a | b;
		return a;
	}

	inline __m128 & operator ^= (__m128 & a, __m128 b)
	{
		a = a ^ b;
		return a;
	}
	inline __m128 & operator ^= (__m128 & a, float b)
	{
		a = a ^ b;
		return a;
	}

	inline __m128 operator == (__m128 a, __m128 b)
	{
		return _mm_cmpeq_ps(a, b);
	}
	inline __m128 operator == (float a, __m128 b)
	{
		return _mm_cmpeq_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator == (__m128 a, float b)
	{
		return _mm_cmpeq_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator != (__m128 a, __m128 b)
	{
		return _mm_cmpneq_ps(a, b);
	}
	inline __m128 operator != (float a, __m128 b)
	{
		return _mm_cmpneq_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator != (__m128 a, float b)
	{
		return _mm_cmpneq_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator < (__m128 a, __m128 b)
	{
		return _mm_cmplt_ps(a, b);
	}
	inline __m128 operator < (float a, __m128 b)
	{
		return _mm_cmplt_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator < (__m128 a, float b)
	{
		return _mm_cmplt_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator > (__m128 a, __m128 b)
	{
		return _mm_cmpgt_ps(a, b);
	}
	inline __m128 operator > (float a, __m128 b)
	{
		return _mm_cmpgt_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator > (__m128 a, float b)
	{
		return _mm_cmpgt_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator <= (__m128 a, __m128 b)
	{
		return _mm_cmple_ps(a, b);
	}
	inline __m128 operator <= (float a, __m128 b)
	{
		return _mm_cmple_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator <= (__m128 a, float b)
	{
		return _mm_cmple_ps(a, _mm_set_ps(b, b, b, b));
	}

	inline __m128 operator >= (__m128 a, __m128 b)
	{
		return _mm_cmpge_ps(a, b);
	}
	inline __m128 operator >= (float a, __m128 b)
	{
		return _mm_cmpge_ps(_mm_set_ps(a, a, a, a), b);
	}
	inline __m128 operator >= (__m128 a, float b)
	{
		return _mm_cmpge_ps(a, _mm_set_ps(b, b, b, b));
	}



	// Overloaded operators for __m128i, so it can be used in vectors

	inline __m128i operator + (__m128i a, __m128i b)
	{
		return _mm_add_epi32(a, b);
	}
	inline __m128i operator + (int a, __m128i b)
	{
		return _mm_add_epi32(_mm_set_epi32(a, a, a, a), b);
	}
	inline __m128i operator + (__m128i a, int b)
	{
		return _mm_add_epi32(a, _mm_set_epi32(b, b, b, b));
	}

	inline __m128i operator - (__m128i a, __m128i b)
	{
		return _mm_sub_epi32(a, b);
	}
	inline __m128i operator - (int a, __m128i b)
	{
		return _mm_sub_epi32(_mm_set_epi32(a, a, a, a), b);
	}
	inline __m128i operator - (__m128i a, int b)
	{
		return _mm_sub_epi32(a, _mm_set_epi32(b, b, b, b));
	}

	inline __m128i operator - (__m128i a)
	{
		return _mm_sub_epi32(_mm_setzero_si128(), a);
	}

	inline __m128i operator * (__m128i a, __m128i b)
	{
		// Note: there's no 32-bit int multiply in SSE2 (there is in SSE4.1).
		// This sequence of shuffles and 64-bit muls emulates it, as seen here:
		// http://stackoverflow.com/questions/10500766/sse-multiplication-of-4-32-bit-integers
		__m128i tmp1 = _mm_mul_epu32(a,b); // mul 2,0
		__m128i tmp2 = _mm_mul_epu32( _mm_srli_si128(a,4), _mm_srli_si128(b,4)); // mul 3,1
		return _mm_unpacklo_epi32(
					_mm_shuffle_epi32(tmp1, _MM_SHUFFLE(0,0,2,0)),
					_mm_shuffle_epi32(tmp2, _MM_SHUFFLE(0,0,2,0)));
	}
	inline __m128i operator * (int a, __m128i b)
	{
		return _mm_set_epi32(a, a, a, a) * b;
	}
	inline __m128i operator * (__m128i a, int b)
	{
		return a * _mm_set_epi32(b, b, b, b);
	}

	// Note: there's no 32-bit integer divide in SSE2.  Could maybe emulate it, or
	// approximate it by converting to float and dividing.
#if 0
	inline __m128i operator / (__m128i a, __m128i b)
	{
		return ???
	}
	inline __m128i operator / (int a, __m128i b)
	{
		return _mm_set_epi32(a, a, a, a) / b;
	}
	inline __m128i operator / (__m128i a, int b)
	{
		return a / _mm_set_epi32(b, b, b, b);
	}
#endif

	inline __m128i operator & (__m128i a, __m128i b)
	{
		return _mm_and_si128(a, b);
	}
	inline __m128i operator & (int a, __m128i b)
	{
		return _mm_and_si128(_mm_set_epi32(a, a, a, a), b);
	}
	inline __m128i operator & (__m128i a, int b)
	{
		return _mm_and_si128(a, _mm_set_epi32(b, b, b, b));
	}

	inline __m128i operator | (__m128i a, __m128i b)
	{
		return _mm_or_si128(a, b);
	}
	inline __m128i operator | (int a, __m128i b)
	{
		return _mm_or_si128(_mm_set_epi32(a, a, a, a), b);
	}
	inline __m128i operator | (__m128i a, int b)
	{
		return _mm_or_si128(a, _mm_set_epi32(b, b, b, b));
	}

	inline __m128i operator ^ (__m128i a, __m128i b)
	{
		return _mm_xor_si128(a, b);
	}
	inline __m128i operator ^ (int a, __m128i b)
	{
		return _mm_xor_si128(_mm_set_epi32(a, a, a, a), b);
	}
	inline __m128i operator ^ (__m128i a, int b)
	{
		return _mm_xor_si128(a, _mm_set_epi32(b, b, b, b));
	}

	inline __m128i operator ! (__m128i a)
	{
		// Note: there's no bitwise-not, so we have to use "andnot" (= ~a & b) with b = true.
		return _mm_andnot_si128(a, _mm_set_epi32(~0, ~0, ~0, ~0));
	}

	inline __m128i operator ~ (__m128i a)
	{
		// Note: there's no bitwise-not, so we have to use "andnot" (= ~a & b) with b = true.
		return _mm_andnot_si128(a, _mm_set_epi32(~0, ~0, ~0, ~0));
	}

	inline __m128i & operator += (__m128i & a, __m128i b)
	{
		a = a + b;
		return a;
	}
	inline __m128i & operator += (__m128i & a, int b)
	{
		a = a + b;
		return a;
	}

	inline __m128i & operator -= (__m128i & a, __m128i b)
	{
		a = a - b;
		return a;
	}
	inline __m128i & operator -= (__m128i & a, int b)
	{
		a = a - b;
		return a;
	}

	inline __m128i & operator *= (__m128i & a, __m128i b)
	{
		a = a * b;
		return a;
	}
	inline __m128i & operator *= (__m128i & a, int b)
	{
		a = a * b;
		return a;
	}

	// No 32-bit integer divide
#if 0
	inline __m128i & operator /= (__m128i & a, __m128i b)
	{
		a = a / b;
		return a;
	}
	inline __m128i & operator /= (__m128i & a, int b)
	{
		a = a / b;
		return a;
	}
#endif

	inline __m128i & operator &= (__m128i & a, __m128i b)
	{
		a = a & b;
		return a;
	}
	inline __m128i & operator &= (__m128i & a, int b)
	{
		a = a & b;
		return a;
	}

	inline __m128i & operator |= (__m128i & a, __m128i b)
	{
		a = a | b;
		return a;
	}
	inline __m128i & operator |= (__m128i & a, int b)
	{
		a = a | b;
		return a;
	}

	inline __m128i & operator ^= (__m128i & a, __m128i b)
	{
		a = a ^ b;
		return a;
	}
	inline __m128i & operator ^= (__m128i & a, int b)
	{
		a = a ^ b;
		return a;
	}

	inline __m128i operator == (__m128i a, __m128i b)
	{
		return _mm_cmpeq_epi32(a, b);
	}
	inline __m128i operator == (int a, __m128i b)
	{
		return _mm_cmpeq_epi32(_mm_set_epi32(a, a, a, a), b);
	}
	inline __m128i operator == (__m128i a, int b)
	{
		return _mm_cmpeq_epi32(a, _mm_set_epi32(b, b, b, b));
	}

	inline __m128i operator != (__m128i a, __m128i b)
	{
		// Note: there's no _mm_cmpneq_epi32, and there's no bitwise-not...
		// so we have to use "andnot" (= ~a & b) with b = true.
		return _mm_andnot_si128(_mm_cmpeq_epi32(a, b), _mm_set_epi32(~0, ~0, ~0, ~0));
	}
	inline __m128i operator != (int a, __m128i b)
	{
		return _mm_set_epi32(a, a, a, a) != b;
	}
	inline __m128i operator != (__m128i a, int b)
	{
		return a != _mm_set_epi32(b, b, b, b);
	}

	inline __m128i operator < (__m128i a, __m128i b)
	{
		return _mm_cmplt_epi32(a, b);
	}
	inline __m128i operator < (int a, __m128i b)
	{
		return _mm_cmplt_epi32(_mm_set_epi32(a, a, a, a), b);
	}
	inline __m128i operator < (__m128i a, int b)
	{
		return _mm_cmplt_epi32(a, _mm_set_epi32(b, b, b, b));
	}

	inline __m128i operator > (__m128i a, __m128i b)
	{
		return _mm_cmpgt_epi32(a, b);
	}
	inline __m128i operator > (int a, __m128i b)
	{
		return _mm_cmpgt_epi32(_mm_set_epi32(a, a, a, a), b);
	}
	inline __m128i operator > (__m128i a, int b)
	{
		return _mm_cmpgt_epi32(a, _mm_set_epi32(b, b, b, b));
	}

	inline __m128i operator <= (__m128i a, __m128i b)
	{
		// Note: there's no _mm_cmple_epi32, and there's no bitwise-not...
		// so we have to use "andnot" (= ~a & b) with b = true.
		return _mm_andnot_si128(_mm_cmpgt_epi32(a, b), _mm_set_epi32(~0, ~0, ~0, ~0));
	}
	inline __m128i operator <= (int a, __m128i b)
	{
		return _mm_set_epi32(a, a, a, a) <= b;
	}
	inline __m128i operator <= (__m128i a, int b)
	{
		return a <= _mm_set_epi32(b, b, b, b);
	}

	inline __m128i operator >= (__m128i a, __m128i b)
	{
		// Note: there's no _mm_cmpge_epi32, and there's no bitwise-not...
		// so we have to use "andnot" (= ~a & b) with b = true.
		return _mm_andnot_si128(_mm_cmplt_epi32(a, b), _mm_set_epi32(~0, ~0, ~0, ~0));
	}
	inline __m128i operator >= (int a, __m128i b)
	{
		return _mm_set_epi32(a, a, a, a) >= b;
	}
	inline __m128i operator >= (__m128i a, int b)
	{
		return a >= _mm_set_epi32(b, b, b, b);
	}


	
	// Convert memory layouts to and from SIMD-friendly AOSOA layout

	// convertToAOSOA takes a layout like this:
	//   xyz...xyz...xyz...xyz...
	// and makes it look like this:
	//   xxxxyyyyzzzz...xxxxyyyyzzzz...
	// In other words, it groups vectors in chunks, and stores each chunk in SOA layout.
	// Components (xyz) are always assumed to be 32 bits each (float or int).
    // Both input and output layouts can have an arbitrary stride, but the components of each
	// individual vector are assumed to be tightly packed.
	// numComponents is the count of components per vector (e.g. 3 for xyz, 4 for xyzw).
	// numVectors is the count of vectors (each xyz or xyzw).  If this is not a multiple of the,
	// chunk size, the last chunk in the output array will be zero-padded to fill it out.
	// vectorsPerChunk is the number of vectors per chunk. Usually 4 for SSE, 8 for AVX.

	void convertToAOSOA(
		int numComponents,
		int numVectors,
		const void * pInput,
		int inputStrideBytes,
		void * pOutput,
		int outputStrideBytes,
		int vectorsPerChunk = 4);

	// convertFromAOSOA does the inverse of convertToAOSOA.

	void convertFromAOSOA(
		int numComponents,
		int numVectors,
		const void * pInput,
		int inputStrideBytes,
		void * pOutput,
		int outputStrideBytes,
		int vectorsPerChunk = 4);



	// Typedefs of SIMD vectors, matrices, points, and affines,
	// for the most common types and dimensions
	typedef vector<__m128, 2> float2_simd;
	typedef vector<__m128, 3> float3_simd;
	typedef vector<__m128, 4> float4_simd;
	typedef vector<__m128i, 2> int2_simd;
	typedef vector<__m128i, 3> int3_simd;
	typedef vector<__m128i, 4> int4_simd;
	typedef matrix<__m128, 2, 2> float2x2_simd;
	typedef matrix<__m128, 3, 3> float3x3_simd;
	typedef matrix<__m128, 3, 4> float3x4_simd;
	typedef matrix<__m128, 4, 3> float4x3_simd;
	typedef matrix<__m128, 4, 4> float4x4_simd;
	typedef matrix<__m128i, 2, 2> int2x2_simd;
	typedef matrix<__m128i, 3, 3> int3x3_simd;
	typedef matrix<__m128i, 3, 4> int3x4_simd;
	typedef matrix<__m128i, 4, 3> int4x3_simd;
	typedef matrix<__m128i, 4, 4> int4x4_simd;
}
