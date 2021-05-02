#include "pch.h"
#include "Log.h"

std::string Log::SavedOutput;

void Log::WriteLine(const std::string& level, const std::string& message, ConsoleColor color)
{
	SetColor(color);
	
	std::cout << "[" << level << "] " << message << std::endl;
	SavedOutput.append("[" + level + "] " + message + "\n");
	
	SetColor(ConsoleColor::WHITE);
}

void Log::WriteLine(const std::string& message, ConsoleColor color)
{
	SetColor(color);
	
	std::cout << message << std::endl;
	SavedOutput.append(message + "\n");

	SetColor(ConsoleColor::WHITE);
}

void Log::Write(const std::string& level, const std::string& message, ConsoleColor color)
{
	SetColor(color);

	std::cout << "[" << level << "] " << message;
	SavedOutput.append("[" + level + "] " + message);

	SetColor(ConsoleColor::WHITE);
}

void Log::Write(const std::string& message, ConsoleColor color)
{
	SetColor(color);

	std::cout << message;
	SavedOutput.append(message);

	SetColor(ConsoleColor::WHITE);
}

void Log::WaitInput()
{
	WriteLine("?", "Press any key to exit.");
	
	std::cin.get();
}

void Log::SetColor(ConsoleColor color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) color);
}
