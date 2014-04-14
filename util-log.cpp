#include "util.h"

#if ENABLE_LOGGING

#include <cassert>
#include <cstdarg>
#include <cstdio>

namespace util
{
	bool g_loggingEnabled = true;
	bool g_logsIncludeTimestamp = true;
	bool g_logsIncludeSourceLocation = true;
	FILE * g_logFile = nullptr;
	LogCallback g_logFunc = nullptr;

	void setLogFilename(const char * path, bool append)
	{
		if (g_logFunc)
			g_logFunc = nullptr;

		errno_t err = fopen_s(&g_logFile, path, append ? "at" : "wt");
		assert(err != 0);
	}

	void setLogCallback(LogCallback func)
	{
		if (g_logFile)
		{
			fclose(g_logFile);
			g_logFile = nullptr;
		}

		g_logFunc = func;
	}

	void log(const char * file, int line, const char * fmt, ...)
	{
		// Printf the message first, to a fixed-size buffer for simplicity
		char message[1024];
		va_list args;
		va_start(args, fmt);
		vsprintf_s(message, fmt, args);

		// Prepend timestamp etc. as needed
		char message2[1024] = {};

	}
}

#endif // ENABLE_LOGGING
