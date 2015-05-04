#pragma once

// In our parlance, "warnings" are checked only in debug builds and result in a
// log message, while "errors" are checked in all builds and result in a popup.
// Both warnings and errors can trigger debug breaks if desired.

// Warnings can be compiled in or out by #defining ENABLE_WARNINGS to 1 or 0.
// If not defined, warnings default to on in all builds.
// (Note that when compiled in, warnings can be enabled/disabled at runtime.
// Debug breaks are on by default in debug builds only.)
#ifndef ENABLE_WARNINGS
#	define ENABLE_WARNINGS 1
#endif

#if ENABLE_WARNINGS

	namespace util
	{
		extern bool g_breakOnWarning;
	}

	// Emit a warning
#	define WARN(fmt, ...) \
			{ \
				util::log(__FILE__, __LINE__, "Warning: " fmt, __VA_ARGS__); \
				if (util::g_breakOnWarning) __debugbreak(); \
			}

	// Asserts that emit a warning if failed
#	define ASSERT_WARN(f) \
			{ \
				if (!(f)) \
				{ \
					util::log(__FILE__, __LINE__, "Warning: assertion failed: %s", #f); \
					if (util::g_breakOnWarning) __debugbreak(); \
				} \
			}
#	define ASSERT_WARN_MSG(f, fmt, ...) \
			{ \
				if (!(f)) \
				{ \
					util::log(__FILE__, __LINE__, "Warning: " fmt, __VA_ARGS__); \
					if (util::g_breakOnWarning) __debugbreak(); \
				} \
			}

	// Checks evaluate the checked expression in all builds,
	// but only emit a warning if warnings are enabled
#	define CHECK_WARN(f) \
			ASSERT_WARN(f)
#	define CHECK_WARN_MSG(f, fmt, ...) \
			ASSERT_WARN_MSG(f, fmt, __VA_ARGS__)

#else // !ENABLE_WARNINGS

	namespace util
	{
		static const bool g_breakOnWarning = false;
	}

#	define WARN(...)					{}
#	define ASSERT_WARN(...)				{}
#	define ASSERT_WARN_MSG(...)			{}
#	define CHECK_WARN(f)				(void)(f)
#	define CHECK_WARN_MSG(f, fmt, ...)	(void)(f)

#endif // !ENABLE_WARNINGS



// Errors can be compiled in or out by #defining ENABLE_WARNINGS to 1 or 0.
// If not defined, errors default to on in all builds.
// (Note that when compiled in, errors can be enabled/disabled at runtime.
// Debug breaks are on by default in debug builds only.)
#ifndef ENABLE_ERRORS
#	define ENABLE_ERRORS 1
#endif

#if ENABLE_ERRORS

	namespace util
	{
		// Note: if break-on-error is enabled, it suppresses the error callback
		extern bool g_breakOnError;

		typedef void (*ErrorCallback)(const char * message);
		extern ErrorCallback g_errorCallback;

		// Emit a log message
		void error(const char * file, int line, const char * fmt, ...);
		void errorva(const char * file, int line, const char * fmt, va_list args);
	}

	// Emit an error
#	define ERR(fmt, ...) \
			{ \
				util::error(__FILE__, __LINE__, "Error: " fmt, __VA_ARGS__); \
				if (util::g_breakOnError) __debugbreak(); \
			}

	// Asserts that emit an error if failed
#	define ASSERT_ERR(f) \
			{ \
				if (!(f)) \
				{ \
					util::error(__FILE__, __LINE__, "Error: assertion failed: %s", #f); \
					if (util::g_breakOnError) __debugbreak(); \
				} \
			}
#	define ASSERT_ERR_MSG(f, fmt, ...) \
			{ \
				if (!(f)) \
				{ \
					util::error(__FILE__, __LINE__, "Error: " fmt, __VA_ARGS__); \
					if (util::g_breakOnError) __debugbreak(); \
				} \
			}

	// Checks evaluate the checked expression in all builds,
	// but only emit an error if errors are enabled
#	define CHECK_ERR(f) \
			ASSERT_ERR(f)
#	define CHECK_ERR_MSG(f, fmt, ...) \
			ASSERT_ERR_MSG(f, fmt, __VA_ARGS__)

#else // !ENABLE_ERRORS

	namespace util
	{
		static const bool g_breakOnError = false;
		typedef void (*ErrorCallback)(const char * message);
		static const ErrorCallback g_errorCallback = nullptr;
		inline void error(const char * file, int line, const char * fmt, ...) { (void)file; (void)line; (void)fmt; }
		inline void errorva(const char * file, int line, const char * fmt, va_list args) { (void)file; (void)line; (void)fmt; (void)args; }
	}

#	define ERR(...)						{}
#	define ASSERT_ERR(...)				{}
#	define ASSERT_ERR_MSG(...)			{}
#	define CHECK_ERR(f)					(void)(f)
#	define CHECK_ERR_MSG(f, fmt, ...)	(void)(f)

#endif // !ENABLE_ERRORS