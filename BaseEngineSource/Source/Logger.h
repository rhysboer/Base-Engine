#pragma once
#include <cstdio>

namespace BE::Logger {
	inline void Log(const char* message, const char* extension = "") { printf("[LOG] : %s %s\n", message, extension); }
	inline void Warning(const char* message, const char* extension = "") { printf("[WARNING] : %s %s\n", message, extension); }
	inline void Error(const char* message, const char* extension = "") { printf("[ERROR] : %s %s\n", message, extension); }
}

