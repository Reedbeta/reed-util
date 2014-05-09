#include "util-math.h"

namespace util
{
	// Projection matrix implementations

	float4x4 orthoProjD3DStyle(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (zFar - zNear);
		return makefloat4x4(
					2.0f * xScale, 0, 0, 0,
					0, 2.0f * yScale, 0, 0,
					0, 0, -zScale, 0,
					-(left + right) * xScale, -(bottom + top) * yScale, -zNear * zScale, 1);
	}

	float4x4 orthoProjOGLStyle(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (zFar - zNear);
		return makefloat4x4(
					2.0f * xScale, 0, 0, 0,
					0, 2.0f * yScale, 0, 0,
					0, 0, -2.0f * zScale, 0,
					-(left + right) * xScale, -(bottom + top) * yScale, -(zNear + zFar) * zScale, 1);
	}

	float4x4 perspProjD3DStyle(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (zFar - zNear);
		return makefloat4x4(
					2.0f * zNear * xScale, 0, 0, 0,
					0, 2.0f * zNear * yScale, 0, 0,
					(left + right) * xScale, (bottom + top) * yScale, -zFar * zScale, -1,
					0, 0, -zNear * zFar * zScale, 0);
	}

	float4x4 perspProjOGLStyle(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (zFar - zNear);
		return makefloat4x4(
					2.0f * zNear * xScale, 0, 0, 0,
					0, 2.0f * zNear * yScale, 0, 0,
					(left + right) * xScale, (bottom + top) * yScale, -(zNear + zFar) * zScale, -1,
					0, 0, -2.0f * zNear * zFar * zScale, 0);
	}

	float4x4 perspProjD3DStyle(float verticalFOV, float aspect, float zNear, float zFar)
	{
		float yScale = 1.0f / tanf(0.5f * verticalFOV);
		float xScale = yScale / aspect;
		float zScale = 1.0f / (zFar - zNear);
		return makefloat4x4(
					xScale, 0, 0, 0,
					0, yScale, 0, 0,
					0, 0, -zFar * zScale, -1,
					0, 0, -zNear * zFar * zScale, 0);
	}

	float4x4 perspProjOGLStyle(float verticalFOV, float aspect, float zNear, float zFar)
	{
		float yScale = 1.0f / tanf(0.5f * verticalFOV);
		float xScale = yScale / aspect;
		float zScale = 1.0f / (zFar - zNear);
		return makefloat4x4(
					xScale, 0, 0, 0,
					0, yScale, 0, 0,
					0, 0, -(zNear + zFar) * zScale, -1,
					0, 0, -2.0f * zNear * zFar * zScale, 0);
	}
}
