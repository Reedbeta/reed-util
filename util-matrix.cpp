#include "util.h"

namespace util
{
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
}
