// FPU vs SSE speed test

#include <cassert>
#include <cmath>
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <xmmintrin.h>

void calcWorldToCamera_FPU(float yaw, float pitch, const float cameraPos[3], float outWorldToCamera[4][4]);
void calcWorldToCamera_SSE(float yaw, float pitch, const __m128 & cameraPos, __m128 outWorldToCamera[4]);

struct AABB { float mins[3]; float maxs[3]; };
void transformAABBs_FPU(int n, const AABB * inBBs, const float mat[][4][4], AABB * outBBs);

struct AABBs_soa
{
	__m128 * xMins, * yMins, * zMins, * xMaxs, * yMaxs, * zMaxs;
};
struct mats_soa
{
	const __m128 * comps[4][4];
};
void transformAABBs_SSE(int n, AABBs_soa inBBs, mats_soa mats, AABBs_soa outBBs);

// RAII timer
class CTimer
{
public:
	__int64 qpcStart;
	CTimer()
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&qpcStart);
	}
	~CTimer()
	{
		__int64 qpcEnd, qpcFreq;
		QueryPerformanceCounter((LARGE_INTEGER *)&qpcEnd);
		QueryPerformanceFrequency((LARGE_INTEGER *)&qpcFreq);
		printf("%0.1f ms\n", float(qpcEnd - qpcStart) / float(qpcFreq) * 1000.0f);
	}
};

int main()
{
	{
		float yaw = 0.74f;
		float pitch = 0.47f;
		float cameraPos[] = { 1.0f, 2.0f, 3.0f };
		__m128 cameraPos_SSE = { 1.0f, 2.0f, 3.0f };
		float worldToCamera[4][4];
		__m128 worldToCamera_SSE[4];

#ifdef _DEBUG
		int trials = 1000000;
#else
		int trials = 10000000;
#endif

		printf("calcWorldToCamera_FPU %dM times: ", trials/1000000);
		{
			CTimer timer;
			for (int i = 0; i < trials; ++i)
				calcWorldToCamera_FPU(yaw, pitch, cameraPos, worldToCamera);
		}

		printf("calcWorldToCamera_SSE %dM times: ", trials/1000000);
		{
			CTimer timer;
			for (int i = 0; i < trials; ++i)
				calcWorldToCamera_SSE(yaw, pitch, cameraPos_SSE, worldToCamera_SSE);
		}

		assert(memcmp(worldToCamera, worldToCamera_SSE, sizeof(float)*4*4) == 0);
	}

	{
		const int n = 1000;
		AABB inAABBs[n];
		float mats[n][4][4];
		AABB outAABBs[n];

		__m128 inXMins[n/4];
		__m128 inYMins[n/4];
		__m128 inZMins[n/4];
		__m128 inXMaxs[n/4];
		__m128 inYMaxs[n/4];
		__m128 inZMaxs[n/4];
		AABBs_soa inAABBs_soa = { inXMins, inYMins, inZMins, inXMaxs, inYMaxs, inZMaxs };

		__m128 mats_SSE[4][4][n/4];
		mats_soa mats_SSE_soa;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				mats_SSE_soa.comps[i][j] = mats_SSE[i][j];

		__m128 outXMins[n/4];
		__m128 outYMins[n/4];
		__m128 outZMins[n/4];
		__m128 outXMaxs[n/4];
		__m128 outYMaxs[n/4];
		__m128 outZMaxs[n/4];
		AABBs_soa outAABBs_soa = { outXMins, outYMins, outZMins, outXMaxs, outYMaxs, outZMaxs };

		// Fill both sets of inputs with some data
		for (int i = 0; i < n; ++i)
		{
			// !!!UNDONE
		}

#ifdef _DEBUG
		int trials = 1000;
#else
		int trials = 10000;
#endif

		printf("transformAABBs_FPU, %dK AABBs, %dK times: ", n/1000, trials/1000);
		{
			CTimer timer;
			for (int i = 0; i < trials; ++i)
				transformAABBs_FPU(n, inAABBs, mats, outAABBs);
		}

		printf("transformAABBs_SSE, %dK AABBs, %dK times: ", n/1000, trials/1000);
		{
			CTimer timer;
			for (int i = 0; i < trials; ++i)
				transformAABBs_SSE(n, inAABBs_soa, mats_SSE_soa, outAABBs_soa);
		}
	}

	return 0;
}
