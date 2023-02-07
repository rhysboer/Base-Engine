#pragma once
#include <cstdio>
#include <stdio.h>
#include <stdarg.h>
#include <cstring>
#include <assert.h>

#define LOG_BUFFER_LENGTH 1024
#define BE_LOG(log, ...) BE::Logging::Log(log, __VA_ARGS__)
#define BE_WARNING(log, ...) BE::Logging::Warning(log, __VA_ARGS__)
#define BE_ERROR(log, ...) BE::Logging::Error(log, __VA_ARGS__)
#define BE_ASSERT(condition, log, ...) BE::Logging::Assert(condition, log, __FILE__, __LINE__, __VA_ARGS__)

namespace BE::Logging {
	// TODO, clean up this shit

	inline void Error(const char* log, ...) {
		char buffer[LOG_BUFFER_LENGTH];
		va_list args;

		va_start(args, log);
		int e = vsprintf_s(buffer, LOG_BUFFER_LENGTH, log, args);
		if(e < 0)
			perror(buffer);
		va_end(args);

		int i = std::strlen(buffer);
		if(i >= LOG_BUFFER_LENGTH - 2) {
			buffer[LOG_BUFFER_LENGTH - 2] = '\n';
		} else {
			buffer[i] = '\n';
			buffer[i + 1] = '\0';
		}

		printf("[ERROR] : ");
		printf(buffer);
	}

	inline void Warning(const char* log, ...) {
		char buffer[LOG_BUFFER_LENGTH];
		va_list args;

		va_start(args, log);
		int e = vsprintf_s(buffer, LOG_BUFFER_LENGTH, log, args);
		if(e < 0)
			perror(buffer);
		va_end(args);

		int i = std::strlen(buffer);
		if(i >= LOG_BUFFER_LENGTH - 2) {
			buffer[LOG_BUFFER_LENGTH - 2] = '\n';
		} else {
			buffer[i] = '\n';
			buffer[i + 1] = '\0';
		}

		printf("[WARNING] : ");
		printf(buffer);
	}

	inline void Log(const char* log, ...) {
		char buffer[LOG_BUFFER_LENGTH];
		va_list args;
		
		va_start(args, log);
		int e = vsprintf_s(buffer, LOG_BUFFER_LENGTH, log, args);
		if(e < 0)
			perror(buffer);
		va_end(args);
		
		int i = std::strlen(buffer);
		if(i >= LOG_BUFFER_LENGTH - 2) {
			buffer[LOG_BUFFER_LENGTH - 2] = '\n';
		} else {
			buffer[i] = '\n';
			buffer[i + 1] = '\0';
		}
		
		printf("[LOG] : ");
		printf(buffer);
	}

	inline void Assert(const bool& condition, const char* log, const char* _file, const int& _line, ...) {
		if (condition)
			return;

		char buffer[LOG_BUFFER_LENGTH];
		va_list args;

		va_start(args, log);
		int e = vsprintf_s(buffer, LOG_BUFFER_LENGTH, log, args);
		if (e < 0)
			perror(buffer);
		va_end(args);

		int i = std::strlen(buffer);
		if (i >= LOG_BUFFER_LENGTH - 2) {
			buffer[LOG_BUFFER_LENGTH - 2] = '\n';
		} else {
			buffer[i] = '\n';
			buffer[i + 1] = '\0';
		}

		printf("[FATAL ERROR] : %s - File %s line %i\n", buffer, _file, _line);

		__debugbreak();
		assert(false);
	}
}

#undef LOG_BUFFER_LENGTH