#pragma once

namespace util
{
	struct RNG
	{
		uint m_state;

		void seed(uint seed);
		void seedWithTime();

		uint randUint();
		int randInt(int min, int max);
		float randFloat();
		float randFloat(float min, float max);
	};
}
