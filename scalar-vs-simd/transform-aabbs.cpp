#include <algorithm>
#include <cmath>
#include <cstdio>
#include <float.h>

using std::min;
using std::max;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <xmmintrin.h>

/*
	Test of transforming many bounding boxes by matrices and constructing new AABBs.

	Here's how it might look with a vector math lib (written naively):

	struct AABB { float3 mins, pos3 maxs };
	void transformAABBs(int n, const AABB * inBBs, const float4x4 * mats, AABB * outBBs)
	{
		for (int i = 0; i < n; ++i)
		{
			float3 newMins(FLT_MAX), newMaxs(-FLT_MAX);
			for (int j = 0; j < 8; ++j)
			{
				float3 corner;
				corner.x = ((j & 1) != 0) ? inBBs[i].maxs.x : inBBs[i].mins.x;
				corner.y = ((j & 2) != 0) ? inBBs[i].maxs.y : inBBs[i].mins.y;
				corner.z = ((j & 4) != 0) ? inBBs[i].maxs.z : inBBs[i].mins.z;
				corner = corner * mats[i];
				newMins = min(newMins, corner);
				newMaxs = max(newMaxs, corner);
			}
			outBBs[i].mins = newMins;
			outBBs[i].maxs = newMaxs;
		}
	}
*/

struct AABB { float mins[3]; float maxs[3]; };
void transformAABBs_scalar(int n, const AABB * inBBs, const float mats[][4][4], AABB * outBBs)
{
	for (int i = 0; i < n; ++i)
	{
		float newMins[] = { FLT_MAX, FLT_MAX, FLT_MAX };
		float newMaxs[] = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
		for (int j = 0; j < 8; ++j)
		{
			float corner[] =
			{
				((j & 1) != 0) ? inBBs[i].maxs[0] : inBBs[i].mins[0],
				((j & 2) != 0) ? inBBs[i].maxs[1] : inBBs[i].mins[1],
				((j & 4) != 0) ? inBBs[i].maxs[2] : inBBs[i].mins[2],
			};
			float cornerTransformed[] =
			{
				corner[0] * mats[i][0][0] + corner[1] * mats[i][1][0] + corner[2] * mats[i][2][0] + mats[i][3][0],
				corner[0] * mats[i][0][1] + corner[1] * mats[i][1][1] + corner[2] * mats[i][2][1] + mats[i][3][1],
				corner[0] * mats[i][0][2] + corner[1] * mats[i][1][2] + corner[2] * mats[i][2][2] + mats[i][3][2],
			};
			newMins[0] = min(newMins[0], cornerTransformed[0]);
			newMins[1] = min(newMins[1], cornerTransformed[1]);
			newMins[2] = min(newMins[2], cornerTransformed[2]);
			newMaxs[0] = max(newMaxs[0], cornerTransformed[0]);
			newMaxs[1] = max(newMaxs[1], cornerTransformed[1]);
			newMaxs[2] = max(newMaxs[2], cornerTransformed[2]);
		}
		outBBs[i].mins[0] = newMins[0];
		outBBs[i].mins[1] = newMins[1];
		outBBs[i].mins[2] = newMins[2];
		outBBs[i].maxs[0] = newMaxs[0];
		outBBs[i].maxs[1] = newMaxs[1];
		outBBs[i].maxs[2] = newMaxs[2];
	}
}



struct AABB_aos { __m128 mins, maxs; };
void transformAABBs_SSE_aos(int n, const AABB_aos * inBBs, const __m128 mats[][4], AABB_aos * outBBs)
{
	for (int i = 0; i < n; ++i)
	{
		__m128 newMins = { FLT_MAX, FLT_MAX, FLT_MAX };
		__m128 newMaxs = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
		// Note: constructing the corners can be done more efficiently with shuffles,
		// but in a typical vector math lib, this is what you're likely to get
		for (int j = 0; j < 8; ++j)
		{
			__m128 corner =
			{
				((j & 1) != 0) ? inBBs[i].maxs.m128_f32[0] : inBBs[i].mins.m128_f32[0],
				((j & 2) != 0) ? inBBs[i].maxs.m128_f32[1] : inBBs[i].mins.m128_f32[1],
				((j & 4) != 0) ? inBBs[i].maxs.m128_f32[2] : inBBs[i].mins.m128_f32[2],
			};
			// Multiply by matrix
			__m128 cornerX = _mm_shuffle_ps(corner, corner, _MM_SHUFFLE(0,0,0,0));
			__m128 cornerY = _mm_shuffle_ps(corner, corner, _MM_SHUFFLE(1,1,1,1));
			__m128 cornerZ = _mm_shuffle_ps(corner, corner, _MM_SHUFFLE(2,2,2,2));
			__m128 cornerTransformed = _mm_setzero_ps();
			cornerTransformed = _mm_add_ps(cornerTransformed, _mm_mul_ps(cornerX, mats[i][0]));
			cornerTransformed = _mm_add_ps(cornerTransformed, _mm_mul_ps(cornerY, mats[i][1]));
			cornerTransformed = _mm_add_ps(cornerTransformed, _mm_mul_ps(cornerZ, mats[i][2]));
			cornerTransformed = _mm_add_ps(cornerTransformed, mats[i][3]);
			newMins = _mm_min_ps(newMins, cornerTransformed);
			newMaxs = _mm_max_ps(newMaxs, cornerTransformed);
		}
		outBBs[i].mins = newMins;
		outBBs[i].maxs = newMaxs;

		// Here's an implementation with shuffles, for reference,
		// which is a great deal faster than the non-shuffle version
		// (but not as fast as SOA).
/*
		// shuffle (xyzw, XYZW) to (xyXY, zwZW)
		__m128 xyXY = _mm_shuffle_ps(inBBs[i].mins, inBBs[i].maxs, _MM_SHUFFLE(1,0,1,0));
		__m128 zwZW = _mm_shuffle_ps(inBBs[i].mins, inBBs[i].maxs, _MM_SHUFFLE(3,2,3,2));
		// thence shuffle to (xyzw, Xyzw, xYzw, XYzw, xyZw, XyZw, xYZw, XYZw)
		__m128 corner[] = 
		{
			_mm_shuffle_ps(xyXY, zwZW, _MM_SHUFFLE(1,0,1,0)),
			_mm_shuffle_ps(xyXY, zwZW, _MM_SHUFFLE(1,0,1,2)),
			_mm_shuffle_ps(xyXY, zwZW, _MM_SHUFFLE(1,0,3,0)),
			_mm_shuffle_ps(xyXY, zwZW, _MM_SHUFFLE(1,0,3,2)),
			_mm_shuffle_ps(xyXY, zwZW, _MM_SHUFFLE(1,2,1,0)),
			_mm_shuffle_ps(xyXY, zwZW, _MM_SHUFFLE(1,2,1,2)),
			_mm_shuffle_ps(xyXY, zwZW, _MM_SHUFFLE(1,2,3,0)),
			_mm_shuffle_ps(xyXY, zwZW, _MM_SHUFFLE(1,2,3,2)),
		};
*/
	}
}



struct AABBs_soa { __m128 * xMins, * yMins, * zMins, * xMaxs, * yMaxs, * zMaxs; };
struct mats_soa { const __m128 * comps[4][4]; };
void transformAABBs_SSE_soa(int n, AABBs_soa inBBs, mats_soa mats, AABBs_soa outBBs)
{
	// Do 4 BBs at a time
	int nSIMD = (n + 3) / 4;
	for (int i = 0; i < nSIMD; ++i)
	{
		__m128 xMinsNew = { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };
		__m128 yMinsNew = { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };
		__m128 zMinsNew = { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };
		__m128 xMaxsNew = { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };
		__m128 yMaxsNew = { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };
		__m128 zMaxsNew = { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };

		for (int j = 0; j < 8; ++j)
		{
			__m128 xsCorner = ((j & 1) != 0) ? inBBs.xMaxs[i] : inBBs.xMins[i];
			__m128 ysCorner = ((j & 2) != 0) ? inBBs.yMaxs[i] : inBBs.yMins[i];
			__m128 zsCorner = ((j & 4) != 0) ? inBBs.zMaxs[i] : inBBs.zMins[i];

			__m128 xsCornerTransformed = _mm_add_ps(
											_mm_add_ps(_mm_mul_ps(xsCorner, mats.comps[0][0][i]),
													   _mm_mul_ps(ysCorner, mats.comps[1][0][i])),
											_mm_add_ps(_mm_mul_ps(zsCorner, mats.comps[2][0][i]),
																			mats.comps[3][0][i]));
			__m128 ysCornerTransformed = _mm_add_ps(
											_mm_add_ps(_mm_mul_ps(xsCorner, mats.comps[0][1][i]),
													   _mm_mul_ps(ysCorner, mats.comps[1][1][i])),
											_mm_add_ps(_mm_mul_ps(zsCorner, mats.comps[2][1][i]),
																			mats.comps[3][1][i]));
			__m128 zsCornerTransformed = _mm_add_ps(
											_mm_add_ps(_mm_mul_ps(xsCorner, mats.comps[0][2][i]),
													   _mm_mul_ps(ysCorner, mats.comps[1][2][i])),
											_mm_add_ps(_mm_mul_ps(zsCorner, mats.comps[2][2][i]),
																			mats.comps[3][2][i]));

			xMinsNew = _mm_min_ps(xMinsNew, xsCornerTransformed);
			yMinsNew = _mm_min_ps(yMinsNew, ysCornerTransformed);
			zMinsNew = _mm_min_ps(zMinsNew, zsCornerTransformed);
			xMaxsNew = _mm_max_ps(xMaxsNew, xsCornerTransformed);
			yMaxsNew = _mm_max_ps(yMaxsNew, ysCornerTransformed);
			zMaxsNew = _mm_max_ps(zMaxsNew, zsCornerTransformed);
		}

		outBBs.xMins[i] = xMinsNew;
		outBBs.yMins[i] = yMinsNew;
		outBBs.zMins[i] = zMinsNew;
		outBBs.xMaxs[i] = xMaxsNew;
		outBBs.yMaxs[i] = yMaxsNew;
		outBBs.zMaxs[i] = zMaxsNew;
	}

	(void)mats;
}
