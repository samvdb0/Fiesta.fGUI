#include "pch.h"
#include "Config.h"

#include <ini.h>

void Config::Load(const char* fileName)
{	
	mINI::INIFile file(fileName);
	mINI::INIStructure data;
	
	if (!file.read(data))
	{
		GenerateDefault(fileName);
		file.read(data);
	}

	if (data.has("Features"))
	{
		if (data["Features"]["MenuHotkey"].empty())
		{
			data["Features"]["MenuHotkey"] = std::to_string(0x2D);
		}
		
		Data::MenuHotkey = std::stoul(data["Features"]["MenuHotkey"], nullptr, 0);
	}
	else
	{
		GenerateDefault(fileName);
	}
}

void Config::Save(const char* fileName)
{
	Log::Write("!", "Saving config... ");
	
	mINI::INIFile file(fileName);
	mINI::INIStructure data;

	data["Features"]["MenuHotkey"] = std::to_string(Data::MenuHotkey);
	
	if (!file.generate(data))
	{
		Log::WriteLine("failed", ConsoleColor::RED);
	}
	else
	{
		Log::WriteLine("success!", ConsoleColor::GREEN);
	}
}

void Config::GenerateDefault(const char* fileName)
{
	Log::Write("!", "Generating default config... ");
	
	mINI::INIFile file(fileName);
	mINI::INIStructure data;

	data["Features"]["MenuHotkey"] = std::to_string(0x2D);
	
	if (!file.generate(data))
	{
		Log::WriteLine("failed", ConsoleColor::RED);
	}
	else
	{
		Log::WriteLine("success!", ConsoleColor::GREEN);
	}
}
