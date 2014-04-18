#include "util.h"

#if ENABLE_LOGGING

#include <cstdarg>
#include <cstdio>
#include <ctime>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace util
{
	bool g_loggingEnabled = true;
	bool g_logsIncludeTimestamp = true;
	bool g_logsIncludeSourceLocation = true;
	FILE * g_logFile = nullptr;
	LogCallback g_logCallback = &OutputDebugStringA;

	void setLogFilename(const char * path, bool append)
	{
		if (g_logFile)
		{
			fclose(g_logFile);
			g_logFile = nullptr;
		}

		CHECK_ERR(fopen_s(&g_logFile, path, append ? "at" : "wt") == 0);
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
		if (g_logsIncludeTimestamp)
		{
			time_t curTime = time(nullptr);
			tm curTm;
			CHECK_WARN(localtime_s(&curTm, &curTime) == 0);
			strftime(message2, dim(message2), "[%Y-%m-%d %H:%M:%S] ", &curTm);
		}
		if (g_logsIncludeSourceLocation)
		{
			sprintf_s(message2, "%s[%s:%d] ", message2, file, line);
		}
		sprintf_s(message2, "%s%s\n", message2, message);

		// Write to file and/or send to callback
		if (g_logFile)
		{
			fputs(message2, g_logFile);
			fflush(g_logFile);
		}
		if (g_logCallback)
		{
			g_logCallback(message2);
		}
	}
}

#endif // ENABLE_LOGGING
