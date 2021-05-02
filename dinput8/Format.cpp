#include "pch.h"
#include "Format.h"

char* Format::Create(const char* fmt, ...)
{
	static char buff[2048];
	va_list args;

	va_start(args, fmt);
	vsnprintf(buff, 2048, fmt, args);
	va_end(args);

	return buff;
}
