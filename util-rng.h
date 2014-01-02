#pragma once

namespace util
{
	// Thomas Wang's fast 32-bit integer hash
	uint wangHash(uint x);

	// Fast RNG using Xorshift algorithm
	struct RNG
	{
		uint m_state;

		void seed(uint seed);
		void seed();

		uint randUint();
		int randInt(int min, int max);
		float randFloat();
		float randFloat(float min, float max);
	};
}
