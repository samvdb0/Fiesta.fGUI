#pragma once
#include <string>

enum class ConsoleColor
{
	GREEN = 10,
	RED = 12,
	WHITE = 15,
};

class Log
{
public:
	__declspec(noinline) static void WriteLine(const std::string& level, const std::string& message, ConsoleColor color = ConsoleColor::WHITE);
	__declspec(noinline) static void WriteLine(const std::string& message, ConsoleColor color = ConsoleColor::WHITE);
	__declspec(noinline) static void Write(const std::string& level, const std::string& message, ConsoleColor color = ConsoleColor::WHITE);
	__declspec(noinline) static void Write(const std::string& message, ConsoleColor color = ConsoleColor::WHITE);

	__declspec(noinline) static void WaitInput();
	
	__declspec(noinline) static void SetColor(ConsoleColor color);

	static std::string SavedOutput;
};