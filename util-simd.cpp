#include "util.h"
#include <cassert>

namespace util
{
	// Convert memory layouts to and from SIMD-friendly (AOSOA) layout

	void convertToSIMD(
			uint numComponents,
			uint numVectors,
			const void * pInput,
			uint inputStrideBytes,
			void * pOutput,
			uint outputStrideBytes)
	{
		static const int simdWidth = 4;

		assert(numComponents > 0);
		assert(pInput);
		assert(inputStrideBytes >= sizeof(float) * numComponents);
		assert(pOutput);
		assert((size_t)pOutput % (simdWidth * sizeof(float)) == 0);
		assert(outputStrideBytes >= simdWidth * sizeof(float) * numComponents);

		// Do the part that's a multiple of simdWidth
		for (; numVectors >= simdWidth; numVectors -= simdWidth)
		{
			for (uint i = 0; i < simdWidth; ++i)
			{
				for (uint j = 0; j < numComponents; ++j)
					((float *)pOutput)[simdWidth*j + i] = ((float *)pInput)[j];

				pInput = advanceBytes(pInput, inputStrideBytes);
			}

			pOutput = advanceBytes(pOutput, outputStrideBytes);
		}

		// Do any part left over
		for (uint i = 0; i < numVectors; ++i)
		{
			for (uint j = 0; j < numComponents; ++j)
				((float *)pOutput)[simdWidth*j + i] = ((float *)pInput)[j];

			pInput = advanceBytes(pInput, inputStrideBytes);
		}
	}

	void convertFromSIMD(
			uint numComponents,
			uint numVectors,
			const void * pInput,
			uint inputStrideBytes,
			void * pOutput,
			uint outputStrideBytes)
	{
		static const int simdWidth = 4;

		assert(numComponents > 0);
		assert(pInput);
		assert((size_t)pInput % (simdWidth * sizeof(float)) == 0);
		assert(inputStrideBytes >= simdWidth * sizeof(float) * numComponents);
		assert(pOutput);
		assert(outputStrideBytes >= sizeof(float) * numComponents);

		// Do the part that's a multiple of simdWidth
		for (; numVectors >= simdWidth; numVectors -= simdWidth)
		{
			for (uint i = 0; i < simdWidth; ++i)
			{
				for (uint j = 0; j < numComponents; ++j)
					((float *)pOutput)[j] = ((float *)pInput)[simdWidth*j + i];

				pOutput = advanceBytes(pOutput, outputStrideBytes);
			}

			pInput = advanceBytes(pInput, inputStrideBytes);
		}

		// Do any part left over
		for (uint i = 0; i < numVectors; ++i)
		{
			for (uint j = 0; j < numComponents; ++j)
				((float *)pOutput)[j] = ((float *)pInput)[simdWidth*j + i];

			pOutput = advanceBytes(pOutput, outputStrideBytes);
		}
	}
}
