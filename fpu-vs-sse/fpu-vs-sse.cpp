// FPU vs SSE speed test

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>

using std::min;
using std::max;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <xmmintrin.h>



void calcWorldToCamera_FPU(float yaw, float pitch, const float cameraPos[3], float outWorldToCamera[4][4]);
void calcWorldToCamera_SSE_aos(float yaw, float pitch, const __m128 & cameraPos, __m128 outWorldToCamera[4]);

struct AABB { float mins[3]; float maxs[3]; };
void transformAABBs_FPU(int n, const AABB * inBBs, const float mat[][4][4], AABB * outBBs);

struct AABB_aos { __m128 mins, maxs; };
void transformAABBs_SSE_aos(int n, const AABB_aos * inBBs, const __m128 mats[][4], AABB_aos * outBBs);

struct AABBs_soa { __m128 * xMins, * yMins, * zMins, * xMaxs, * yMaxs, * zMaxs; };
struct mats_soa { const __m128 * comps[4][4]; };
void transformAABBs_SSE_soa(int n, AABBs_soa inBBs, mats_soa mats, AABBs_soa outBBs);



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

// Quick simple RNG based on Xorhash
struct RNG
{
	unsigned int m_state;

	void seed(unsigned int seed)
	{
		// Thomas Wang's integer hash, as reported by Bob Jenkins
		seed = (seed ^ 61) ^ (seed >> 16);
		seed *= 9;
		seed = seed ^ (seed >> 4);
		seed *= 0x27d4eb2d;
		seed = seed ^ (seed >> 15);
		m_state = seed;
	}

	unsigned int randUint()
	{
		// Xorshift algorithm from George Marsaglia's paper
		m_state ^= (m_state << 13);
		m_state ^= (m_state >> 17);
		m_state ^= (m_state << 5);
		return m_state;
	}

	float randFloat()
	{
		return float(randUint()) * (1.0f / 4294967296.0f);
	}

	float randFloat(float min, float max)
	{
		return min + (min - max)*randFloat();
	}
};

int main()
{
	float absEpsilon = 1e-5f;
	float relEpsilon = 1e-5f;

	{
		float yaw = 0.74f;
		float pitch = 0.47f;
		float cameraPos[] = { 1.0f, 2.0f, 3.0f };
		__m128 cameraPos_aos = { 1.0f, 2.0f, 3.0f };
		float worldToCamera[4][4];
		__m128 worldToCamera_aos[4];

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

		printf("calcWorldToCamera_SSE_aos %dM times: ", trials/1000000);
		{
			CTimer timer;
			for (int i = 0; i < trials; ++i)
				calcWorldToCamera_SSE_aos(yaw, pitch, cameraPos_aos, worldToCamera_aos);
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (fabs(worldToCamera[i][j] - worldToCamera_aos[i].m128_f32[j]) >
						max(absEpsilon, relEpsilon * fabs(worldToCamera[i][j])))
				{
					printf("Warning: significant mismatch in component [%d][%d]\n", i, j);
				}
			}
		}
	}

	{
		const int n = 1000;
		AABB inAABBs[n];
		float mats[n][4][4];
		AABB outAABBs[n];

		AABB_aos inAABBs_aos[n];
		__m128 mats_aos[n][4];
		AABB_aos outAABBs_aos[n];

		__m128 inXMins[n/4];
		__m128 inYMins[n/4];
		__m128 inZMins[n/4];
		__m128 inXMaxs[n/4];
		__m128 inYMaxs[n/4];
		__m128 inZMaxs[n/4];
		AABBs_soa inAABBs_soa = { inXMins, inYMins, inZMins, inXMaxs, inYMaxs, inZMaxs };

		__m128 matComps_soa[4][4][n/4];
		mats_soa theMats_soa;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				theMats_soa.comps[i][j] = matComps_soa[i][j];

		__m128 outXMins[n/4];
		__m128 outYMins[n/4];
		__m128 outZMins[n/4];
		__m128 outXMaxs[n/4];
		__m128 outYMaxs[n/4];
		__m128 outZMaxs[n/4];
		AABBs_soa outAABBs_soa = { outXMins, outYMins, outZMins, outXMaxs, outYMaxs, outZMaxs };

		// Fill all three sets of inputs with some randomly generated data
		RNG rng;
		rng.seed(47);
		for (int i = 0; i < n; ++i)
		{
			inAABBs[i].mins[0] = inAABBs_aos[i].mins.m128_f32[0] = inXMins[i/4].m128_f32[i%4] = rng.randFloat(-10, 10);
			inAABBs[i].mins[1] = inAABBs_aos[i].mins.m128_f32[1] = inYMins[i/4].m128_f32[i%4] = rng.randFloat(-10, 10);
			inAABBs[i].mins[2] = inAABBs_aos[i].mins.m128_f32[2] = inZMins[i/4].m128_f32[i%4] = rng.randFloat(-10, 10);
			inAABBs[i].maxs[0] = inAABBs_aos[i].maxs.m128_f32[0] = inXMaxs[i/4].m128_f32[i%4] = inAABBs[i].mins[0] + rng.randFloat(1, 10);
			inAABBs[i].maxs[1] = inAABBs_aos[i].maxs.m128_f32[1] = inYMaxs[i/4].m128_f32[i%4] = inAABBs[i].mins[1] + rng.randFloat(1, 10);
			inAABBs[i].maxs[2] = inAABBs_aos[i].maxs.m128_f32[2] = inZMaxs[i/4].m128_f32[i%4] = inAABBs[i].mins[2] + rng.randFloat(1, 10);

			float translate[] = { rng.randFloat(-10, 10), rng.randFloat(-10, 10), rng.randFloat(-10, 10) };
			calcWorldToCamera_FPU(rng.randFloat(0, 2.0f*3.14159f), rng.randFloat(-3.0f, 3.0f), translate, mats[i]);
			for (int j = 0; j < 4; ++j)
				for (int k = 0; k < 4; ++k)
					matComps_soa[j][k][i/4].m128_f32[i%4] = mats_aos[i][j].m128_f32[k] = mats[i][j][k];
		}

#ifdef _DEBUG
		int trials = 100;
#else
		int trials = 10000;
#endif

		printf("transformAABBs_FPU, %dK AABBs, %dK times: ", n/1000, trials/1000);
		{
			CTimer timer;
			for (int i = 0; i < trials; ++i)
				transformAABBs_FPU(n, inAABBs, mats, outAABBs);
		}

		printf("transformAABBs_SSE_aos, %dK AABBs, %dK times: ", n/1000, trials/1000);
		{
			CTimer timer;
			for (int i = 0; i < trials; ++i)
				transformAABBs_SSE_aos(n, inAABBs_aos, mats_aos, outAABBs_aos);
		}

		printf("transformAABBs_SSE_soa, %dK AABBs, %dK times: ", n/1000, trials/1000);
		{
			CTimer timer;
			for (int i = 0; i < trials; ++i)
				transformAABBs_SSE_soa(n, inAABBs_soa, theMats_soa, outAABBs_soa);
		}

		for (int i = 0; i < n; ++i)
		{
			if (fabs(outAABBs[i].mins[0] - outAABBs_aos[i].mins.m128_f32[0]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].mins[0])) ||
				fabs(outAABBs[i].mins[1] - outAABBs_aos[i].mins.m128_f32[1]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].mins[1])) ||
				fabs(outAABBs[i].mins[2] - outAABBs_aos[i].mins.m128_f32[2]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].mins[2])) ||
				fabs(outAABBs[i].maxs[0] - outAABBs_aos[i].maxs.m128_f32[0]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].maxs[0])) ||
				fabs(outAABBs[i].maxs[1] - outAABBs_aos[i].maxs.m128_f32[1]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].maxs[1])) ||
				fabs(outAABBs[i].maxs[2] - outAABBs_aos[i].maxs.m128_f32[2]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].maxs[2])))
			{
				printf("Warning: significant mismatch in AOS bounding box %d\n", i);
			}
			if (fabs(outAABBs[i].mins[0] - outXMins[i/4].m128_f32[i%4]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].mins[0])) ||
				fabs(outAABBs[i].mins[1] - outYMins[i/4].m128_f32[i%4]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].mins[1])) ||
				fabs(outAABBs[i].mins[2] - outZMins[i/4].m128_f32[i%4]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].mins[2])) ||
				fabs(outAABBs[i].maxs[0] - outXMaxs[i/4].m128_f32[i%4]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].maxs[0])) ||
				fabs(outAABBs[i].maxs[1] - outYMaxs[i/4].m128_f32[i%4]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].maxs[1])) ||
				fabs(outAABBs[i].maxs[2] - outZMaxs[i/4].m128_f32[i%4]) > max(absEpsilon, relEpsilon * fabs(outAABBs[i].maxs[2])))
			{
				printf("Warning: significant mismatch in SOA bounding box %d\n", i);
			}
		}
	}

	return 0;
}
