#include "util.h"

#include <cstdarg>
#include <cstdio>
#include <limits>

namespace util
{
	extern const float infinity = std::numeric_limits<float>::infinity();
	extern const float NaN = std::numeric_limits<float>::quiet_NaN();

	void __declspec(noreturn) exit(const char * fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		::exit(1);
	}
}
