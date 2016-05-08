#include "util-math.h"

namespace util
{
	// Convert memory layouts to and from SIMD-friendly (AOSOA) layout

	void convertToAOSOA(
		int numComponents,
		int numVectors,
		const void * pInput,
		int inputStrideBytes,
		void * pOutput,
		int outputStrideBytes,
		int vectorsPerChunk /*= 4*/)
	{
		ASSERT_ERR(numComponents > 0);
		ASSERT_ERR(pInput);
		ASSERT_ERR(inputStrideBytes >= sizeof(float) * numComponents);
		ASSERT_ERR(pOutput);
		ASSERT_ERR((size_t)pOutput % (vectorsPerChunk * sizeof(float)) == 0);
		ASSERT_ERR(outputStrideBytes >= vectorsPerChunk * sizeof(float) * numComponents);

		// Do the part that's a multiple of vectorsPerChunk
		for (; numVectors >= vectorsPerChunk; numVectors -= vectorsPerChunk)
		{
			for (int i = 0; i < vectorsPerChunk; ++i)
			{
				for (int j = 0; j < numComponents; ++j)
					((float *)pOutput)[vectorsPerChunk*j + i] = ((float *)pInput)[j];

				pInput = offsetPtr(pInput, inputStrideBytes);
			}

			pOutput = offsetPtr(pOutput, outputStrideBytes);
		}

		// Do any part left over
		for (int i = 0; i < numVectors; ++i)
		{
			for (int j = 0; j < numComponents; ++j)
				((float *)pOutput)[vectorsPerChunk*j + i] = ((float *)pInput)[j];

			pInput = offsetPtr(pInput, inputStrideBytes);
		}
	}

	void convertFromAOSOA(
		int numComponents,
		int numVectors,
		const void * pInput,
		int inputStrideBytes,
		void * pOutput,
		int outputStrideBytes,
		int vectorsPerChunk /*= 4*/)
	{
		ASSERT_ERR(numComponents > 0);
		ASSERT_ERR(pInput);
		ASSERT_ERR((size_t)pInput % (vectorsPerChunk * sizeof(float)) == 0);
		ASSERT_ERR(inputStrideBytes >= vectorsPerChunk * sizeof(float) * numComponents);
		ASSERT_ERR(pOutput);
		ASSERT_ERR(outputStrideBytes >= sizeof(float) * numComponents);

		// Do the part that's a multiple of vectorsPerChunk
		for (; numVectors >= vectorsPerChunk; numVectors -= vectorsPerChunk)
		{
			for (int i = 0; i < vectorsPerChunk; ++i)
			{
				for (int j = 0; j < numComponents; ++j)
					((float *)pOutput)[j] = ((float *)pInput)[vectorsPerChunk*j + i];

				pOutput = offsetPtr(pOutput, outputStrideBytes);
			}

			pInput = offsetPtr(pInput, inputStrideBytes);
		}

		// Do any part left over
		for (int i = 0; i < numVectors; ++i)
		{
			for (int j = 0; j < numComponents; ++j)
				((float *)pOutput)[j] = ((float *)pInput)[vectorsPerChunk*j + i];

			pOutput = offsetPtr(pOutput, outputStrideBytes);
		}
	}
}
