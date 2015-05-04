#pragma once

// Logging can be compiled in or out by #defining ENABLE_LOGGING to 1 or 0.
// If not defined, logging defaults to on in all builds.
#ifndef ENABLE_LOGGING
#	define ENABLE_LOGGING 1
#endif

#if ENABLE_LOGGING

	namespace util
	{
		// Set logging destination(s) - log to a file, and/or set a per-message callback
		void setLogFilename(const char * path, bool append = true);
		typedef void (*LogCallback)(const char * message);
		extern LogCallback g_logCallback;

		// Emit a log message
		void log(const char * file, int line, const char * fmt, ...);
		void logva(const char * file, int line, const char * fmt, va_list args);
	}

#	define LOG(fmt, ...) \
			util::log(__FILE__, __LINE__, fmt, __VA_ARGS__)
#	define LOGIF(f, fmt, ...) \
			{ if (f) util::log(__FILE__, __LINE__, fmt, __VA_ARGS__); }

#else // !ENABLE_LOGGING

	namespace util
	{
		inline void setLogFilename(const char * path, bool append = true) { (void)path; (void)append; }
		typedef void (*LogCallback)(const char * message, const char * file, int line);
		static const LogCallback g_logCallback = nullptr;
		inline void log(const char * file, int line, const char * fmt, ...) { (void)file; (void)line; (void)fmt; }
		inline void logva(const char * file, int line, const char * fmt, va_list args) { (void)file; (void)line; (void)fmt; (void)args; }
	}

#	define LOG(...)		{}
#	define LOGIF(...)	{}

#endif // !ENABLE_LOGGING
