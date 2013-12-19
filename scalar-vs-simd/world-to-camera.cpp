#include <cmath>
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <xmmintrin.h>

/*
	Test of calculating a world-to-camera matrix.

	Here's how it might look with a vector math lib:

	void worldToCamera(float yaw, float pitch, const float3 & cameraPos, float4x4 & outWorldToCamera)
	{
		float sinYaw = sin(yaw);
		float cosYaw = cos(yaw);
		float sinPitch = sin(pitch);
		float cosPitch = cos(pitch);

		// Calculate camera basis vectors using spherical coordinates
		float3 vecView = { cosYaw * cosPitch, sinYaw * cosPitch, sinPitch };
		float3 vecUp = { -cosYaw * sinPitch, -sinYaw * sinPitch, cosPitch };
		float3 vecLeft = cross(vecUp, vecView);

		// Create matrix by assembling vectors as columns
		float3x3 matWorldToCamera = makefloat3x3Cols(vecView, vecLeft, vecUp);

		// Calculate inverse translation and put it together in a 4x4
		return float4x4(matWorldToCamera, -cameraPos * matWorldToCamera);
	}
*/

void calcWorldToCamera_scalar(float yaw, float pitch, const float cameraPos[3], float outWorldToCamera[4][4])
{
	float sinYaw = sin(yaw);
	float cosYaw = cos(yaw);
	float sinPitch = sin(pitch);
	float cosPitch = cos(pitch);

	// Calculate camera basis vectors using spherical coordinates
	float vecView[3] = { cosYaw * cosPitch, sinYaw * cosPitch, sinPitch };
	float vecUp[3] = { -cosYaw * sinPitch, -sinYaw * sinPitch, cosPitch };
	float vecLeft[3] =
	{
		vecUp[1]*vecView[2] - vecUp[2]*vecView[1],
		vecUp[2]*vecView[0] - vecUp[0]*vecView[2],
		vecUp[0]*vecView[1] - vecUp[1]*vecView[0],
	};

	// Create matrix by assembling vectors as columns
	memset(outWorldToCamera, 0, sizeof(float)*4*4);
	outWorldToCamera[0][0] = vecView[0]; outWorldToCamera[1][0] = vecView[1]; outWorldToCamera[2][0] = vecView[2];
	outWorldToCamera[0][1] = vecLeft[0]; outWorldToCamera[1][1] = vecLeft[1]; outWorldToCamera[2][1] = vecLeft[2];
	outWorldToCamera[0][2] = vecUp[0];   outWorldToCamera[1][2] = vecUp[1];   outWorldToCamera[2][2] = vecUp[2];

	// Calculate inverse translation, -cameraPos * outWorldToCamera
	float invTranslation[] =
	{
		-(cameraPos[0] * outWorldToCamera[0][0] + cameraPos[1] * outWorldToCamera[1][0] + cameraPos[2] * outWorldToCamera[2][0]),
		-(cameraPos[0] * outWorldToCamera[0][1] + cameraPos[1] * outWorldToCamera[1][1] + cameraPos[2] * outWorldToCamera[2][1]),
		-(cameraPos[0] * outWorldToCamera[0][2] + cameraPos[1] * outWorldToCamera[1][2] + cameraPos[2] * outWorldToCamera[2][2]),
	};

	// Stuff it in the matrix, with final component = 1.0
	outWorldToCamera[3][0] = invTranslation[0];
	outWorldToCamera[3][1] = invTranslation[1];
	outWorldToCamera[3][2] = invTranslation[2];
	outWorldToCamera[3][3] = 1.0f;
}

void calcWorldToCamera_SSE_aos(float yaw, float pitch, const __m128 & cameraPos, __m128 outWorldToCamera[4])
{
	float sinYaw = sin(yaw);
	float cosYaw = cos(yaw);
	float sinPitch = sin(pitch);
	float cosPitch = cos(pitch);

	// Calculate camera basis vectors using spherical coordinates
	__m128 vecView = { cosYaw * cosPitch, sinYaw * cosPitch, sinPitch };
	__m128 vecUp = { -cosYaw * sinPitch, -sinYaw * sinPitch, cosPitch };
	__m128 vecViewShuffle120 = _mm_shuffle_ps(vecView, vecView, _MM_SHUFFLE(0,0,2,1));
	__m128 vecViewShuffle201 = _mm_shuffle_ps(vecView, vecView, _MM_SHUFFLE(0,1,0,2));
	__m128 vecUpShuffle120 = _mm_shuffle_ps(vecUp, vecUp, _MM_SHUFFLE(0,0,2,1));
	__m128 vecUpShuffle201 = _mm_shuffle_ps(vecUp, vecUp, _MM_SHUFFLE(0,1,0,2));
	__m128 vecLeft = _mm_sub_ps(_mm_mul_ps(vecUpShuffle120, vecViewShuffle201),
								_mm_mul_ps(vecUpShuffle201, vecViewShuffle120));

	// Create matrix by assembling vectors as columns
	// (in SSE, easiest to do this by assembling as rows and then transposing)
	outWorldToCamera[0] = vecView;
	outWorldToCamera[1] = vecLeft;
	outWorldToCamera[2] = vecUp;
	outWorldToCamera[3] = _mm_setzero_ps();
	_MM_TRANSPOSE4_PS(outWorldToCamera[0], outWorldToCamera[1], outWorldToCamera[2], outWorldToCamera[3]);

	// Calculate inverse translation, -cameraPos * outWorldToCamera
	__m128 cameraPosX = _mm_shuffle_ps(cameraPos, cameraPos, _MM_SHUFFLE(0,0,0,0));
	__m128 cameraPosY = _mm_shuffle_ps(cameraPos, cameraPos, _MM_SHUFFLE(1,1,1,1));
	__m128 cameraPosZ = _mm_shuffle_ps(cameraPos, cameraPos, _MM_SHUFFLE(2,2,2,2));
	__m128 invTranslation = _mm_setzero_ps();
	invTranslation = _mm_sub_ps(invTranslation, _mm_mul_ps(cameraPosX, outWorldToCamera[0]));
	invTranslation = _mm_sub_ps(invTranslation, _mm_mul_ps(cameraPosY, outWorldToCamera[1]));
	invTranslation = _mm_sub_ps(invTranslation, _mm_mul_ps(cameraPosZ, outWorldToCamera[2]));

	// Stuff it in the matrix, with final component = 1.0
	outWorldToCamera[3] = invTranslation;
	outWorldToCamera[3].m128_f32[3] = 1.0f;
}
