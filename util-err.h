#pragma once

// Errors/warnings can be compiled in or out by #defining ENABLE_ERRORS to 1 or 0.
// If not defined, errors/warnings default to on in debug builds only.
// (Note that when compiled in, errors/warnings can be enabled/disabled at runtime.)
#ifndef ENABLE_ERRORS
#	ifdef _DEBUG
#		define ENABLE_ERRORS 1
#	else
#		define ENABLE_ERRORS 0
#	endif
#endif

#if ENABLE_ERRORS

namespace util
{
	extern bool g_breakOnWarning;
	extern bool g_breakOnError;
}

// Emit a warning
#define WARN(fmt, ...) \
			{ \
				util::log(__FILE__, __LINE__, "Warning: " fmt, __VA_ARGS__); \
				if (util::g_breakOnWarning) __debugbreak(); \
			}

// Asserts that emit a warning if failed
#define ASSERT_WARN(f) \
			{ \
				if (!(f)) \
				{ \
					util::log(__FILE__, __LINE__, "Warning: assertion failed: %s", #f); \
					if (util::g_breakOnWarning) __debugbreak(); \
				} \
			}
#define ASSERT_WARN_MSG(f, fmt, ...) \
			{ \
				if (!(f)) \
				{ \
					util::log(__FILE__, __LINE__, "Warning: " fmt, __VA_ARGS__); \
					if (util::g_breakOnWarning) __debugbreak(); \
				} \
			}

// Checks evaluate the checked expression in all builds,
// but only emit a warning if warnings are enabled
#define CHECK_WARN(f) \
			ASSERT_WARN(f)
#define CHECK_WARN_MSG(f, fmt, ...) \
			ASSERT_WARN_MSG(f, fmt, __VA_ARGS__)

// Emit an error
#define ERR(fmt, ...) \
			{ \
				util::log(__FILE__, __LINE__, "Error: " fmt, __VA_ARGS__); \
				if (util::g_breakOnError) __debugbreak(); \
			}

// Asserts that emit an error if failed
#define ASSERT_ERR(f) \
			{ \
				if (!(f)) \
				{ \
					util::log(__FILE__, __LINE__, "Error: assertion failed: %s", #f); \
					if (util::g_breakOnError) __debugbreak(); \
				} \
			}
#define ASSERT_ERR_MSG(f, fmt, ...) \
			{ \
				if (!(f)) \
				{ \
					util::log(__FILE__, __LINE__, "Error: " fmt, __VA_ARGS__); \
					if (util::g_breakOnError) __debugbreak(); \
				} \
			}

// Checks evaluate the checked expression in all builds,
// but only emit a warning if warnings are enabled
#define CHECK_ERR(f) \
			ASSERT_ERR(f)
#define CHECK_ERR_MSG(f, fmt, ...) \
			ASSERT_ERR_MSG(f, fmt, __VA_ARGS__)

#else // !ENABLE_ERRORS

namespace util
{
	static const bool g_breakOnWarning = false;
	static const bool g_breakOnError = false;
}

#define WARN(...)					{}
#define ASSERT_WARN(...)			{}
#define ASSERT_WARN_MSG(...)		{}
#define CHECK_WARN(f)				f
#define CHECK_WARN_MSG(f, fmt, ...)	f

#define ERR(...)					{}
#define ASSERT_ERR(...)				{}
#define ASSERT_ERR_MSG(...)			{}
#define CHECK_ERR(f)				f
#define CHECK_ERR_MSG(f, fmt, ...)	f

#endif // !ENABLE_ERRORS
