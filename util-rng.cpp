#include "util-rng.h"
#include <ctime>

namespace util
{
	// RNG implementation

	void RNG::seedWithTime()
	{
		seed(uint(time(nullptr)));
	}
}
