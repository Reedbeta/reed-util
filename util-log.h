#pragma once

// Logging can be compiled in or out by #defining ENABLE_LOGGING to 1 or 0.
// If not defined, logging defaults to on in debug builds only.
// (Note that when compiled in, logging can be enabled/disabled at runtime.)
#ifndef ENABLE_LOGGING
#	ifdef _DEBUG
#		define ENABLE_LOGGING 1
#	else
#		define ENABLE_LOGGING 0
#	endif
#endif

#if ENABLE_LOGGING

namespace util
{
	// Turn logging on/off
	extern bool g_loggingEnabled;

	// Set various features of log messages
	extern bool g_logsIncludeTimestamp;
	extern bool g_logsIncludeSourceLocation;

	// Set logging destination(s) - log to a file, and/or set a per-message callback
	void setLogFilename(const char * path, bool append = true);
	typedef void (*LogCallback)(const char * message);
	extern LogCallback g_logCallback;

	// Emit a log message
	void log(const char * file, int line, const char * fmt, ...);
}

#define LOG(fmt, ...) \
			util::log(__FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOGIF(f, fmt, ...) \
			{ if (f) util::log(__FILE__, __LINE__, fmt, __VA_ARGS__); }

#else // !ENABLE_LOGGING

namespace util
{
	static const bool g_loggingEnabled = false;
	static const bool g_logsIncludeTimestamp = false;
	static const bool g_logsIncludeSourceLocation = false;
	inline void setLogFilename(const char * path) { (void)path; }
	typedef void (*LogCallback)(const char * message, const char * file, int line);
	inline void setLogCallback(LogCallback func) { (void)func; }
	inline void log(const char * file, int line, const char * fmt, ...) { (void)file; (void)line; (void)fmt; }
}

#define LOG(...)	{}
#define LOGIF(...)	{}

#endif // !ENABLE_LOGGING
