#pragma once

// Nathan Reed's random number library - Copyright (C) 2013

namespace reed
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
