#include "util.h"

#if ENABLE_WARNINGS

	namespace util
	{
#		ifdef _DEBUG
			bool g_breakOnWarning = true;
#		else
			bool g_breakOnWarning = false;
#		endif
	}

#endif // ENABLE_WARNINGS



#if ENABLE_ERRORS

#	include <cstdio>
#	include <ctime>

#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>

	namespace util
	{
#		ifdef _DEBUG
			bool g_breakOnError = true;
#		else
			bool g_breakOnError = false;
#		endif

		static void DefaultErrorCallback(const char * message);
		ErrorCallback g_errorCallback = &DefaultErrorCallback;

		void error(const char * file, int line, const char * fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			errorva(file, line, fmt, args);
		}

		void errorva(const char * file, int line, const char * fmt, va_list args)
		{
			// Always log the message
			logva(file, line, fmt, args);

			// If not debug-breaking, also send it to the callback
			if (g_errorCallback && !g_breakOnError)
			{
				// Printf the message first, to a fixed-size buffer for simplicity
				char message[1024];
				vsprintf_s(message, fmt, args);

				// Prepend source location
				char message2[1024];
				sprintf_s(message2, "[%s:%d] %s", file, line, message);

				g_errorCallback(message2);
			}
		}

		static void DefaultErrorCallback(const char * message)
		{
			MessageBoxA(nullptr, message, "Error", MB_ICONEXCLAMATION);
		}
	}

#endif // ENABLE_ERRORS
