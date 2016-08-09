#pragma once
#include "util-basics.h"

namespace util
{
	// Thomas Wang's integer hash, as reported by Bob Jenkins
	inline uint wangHash(uint x)
	{
		x = (x ^ 61) ^ (x >> 16);
		x *= 9;
		x = x ^ (x >> 4);
		x *= 0x27d4eb2d;
		x = x ^ (x >> 15);
		return x;
	}

	// Fast RNG using Xorshift algorithm
	struct RNG
	{
		uint m_state;

		RNG() { seedWithTime(); }
		explicit RNG(uint seed_): m_state(wangHash(seed_)) {}

		void seedWithTime();
		void seed(uint seed_) { m_state = wangHash(seed_); }

		uint randUint()
		{
			// Xorshift algorithm from George Marsaglia's paper
			m_state ^= (m_state << 13);
			m_state ^= (m_state >> 17);
			m_state ^= (m_state << 5);
			return m_state;
		}

		int randInt(int min, int max)
		{
			// Note: this doesn't give a uniform distribution if |max - min|
			// isn't a divisor of 2^32.  Problems will be more apparent the
			// larger |max - min| is.  Could use rejection sampling to fix.
			return min + (randUint() % (max - min));
		}

		float randFloat()
		{
			return float(randUint()) * (1.0f / 4294967296.0f);
		}

		float randFloat(float min, float max)
		{
			return lerp(min, max, randFloat());
		}
	};
}
