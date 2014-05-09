#include "util.h"
#include "util-rng.h"
#include <ctime>

namespace util
{
	// RNG implementation

	uint wangHash(uint x)
	{
		// Thomas Wang's integer hash, as reported by Bob Jenkins
		x = (x ^ 61) ^ (x >> 16);
		x *= 9;
		x = x ^ (x >> 4);
		x *= 0x27d4eb2d;
		x = x ^ (x >> 15);
		return x;
	}

	void RNG::seed(uint seed)
	{
		m_state = wangHash(seed);
	}

	void RNG::seed()
	{
		seed(uint(time(nullptr)));
	}

	uint RNG::randUint()
	{
		// Xorshift algorithm from George Marsaglia's paper
		m_state ^= (m_state << 13);
		m_state ^= (m_state >> 17);
		m_state ^= (m_state << 5);
		return m_state;
	}

	int RNG::randInt(int min, int max)
	{
		// Note: this doesn't give a uniform distribution if |max - min|
		// isn't a divisor of 2^32.  Problems will be more apparent the
		// larger |max - min| is.  Could use rejection sampling to fix.
		return min + (randUint() % (max - min));
	}

	float RNG::randFloat()
	{
		return float(randUint()) * (1.0f / 4294967296.0f);
	}

	float RNG::randFloat(float min, float max)
	{
		return lerp(min, max, randFloat());
	}
}
