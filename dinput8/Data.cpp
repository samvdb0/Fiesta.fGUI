#include "pch.h"
#include "Data.h"

#include "Log.h"
#include "Memory.h"

Data::DATA_PTR_STRUCT Data::Pointers;
Data::DATA_ASM_INSTRUCTIONS Data::Instructions;
Data::DATA_CLASS_INSTANCES Data::Instances;

int Data::MenuHotkey;
bool Data::IsIngame;

void Data::Initialize()
{	
	Pointers.IsSuccesful = false;
	
	Log::Write("?", "Attempting to load addresses, this can take a moment... ");

	// Anticheat
	Pointers.Anticheat.LoadXignCode = Memory::FindPatternIDA("55 8B EC 6A FF 68 ? ? ? ? 64 A1 00 00 00 00 50 81 EC E4 02 00 00 A1 ? ? ? ? 33 C5 89 45 F0 53 56 50 8D 45 F4 64 A3 00 00 00 00 6A 1F 33 DB 68 ? ? ? ? 8D 8D 10 FD FF FF C7 85 24 FD FF FF 0F 00 00 00 89 9D 20 FD FF FF 88 9D 10 FD FF FF E8 ? ? ? ? 8D 85 10 FD FF FF 50 68 ? ? ? ? 89 5D FC E8 ? ? ? ? 83 C4 08 83 BD 24 FD FF FF 10 C7 45 FC FF FF FF FF 72 0F 8B 8D 10 FD FF FF 51 E8 ? ? ? ? 83 C4 04 8B");
	
	// GameFrameWork
	Pointers.GameFrameWork.Constructor = Memory::FindPatternIDA("55 8B EC 6A FF 68 ? ? ? ? 64 A1 00 00 00 00 50 83 EC 24 53 56 57 A1 ? ? ? ? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 8B F1 89 75 E8 33 C9 33 DB B8 01 00 00 00 BA 14 00 00 00 F7 E2 0F 90 C1 89 5E 04 89 5E 08 C7 06 ? ? ? ? F7 D9 0B C8 51 E8");
	Pointers.GameFrameWork.InitWindow = Memory::FindPatternIDA("55 8B EC 81 EC 10 01 00 00 A1 ? ? ? ? 33 C5 89 45 FC 56 8B F1 68 00 08 00 00 8D 86 9C 0A 00 00 6A 00");
	Pointers.GameFrameWork.TerminateWindow = Memory::FindPatternIDA("55 8B EC 51 56 8B F1 57 8D 8E A8 02 00 00 E8");
	
	Pointers.GameFrameWork.ProcessNetMsg = Memory::FindPatternIDA("55 8B EC 83 EC 08 A1 ? ? ? ? 33 C5 89 45 FC 8B 45 14");
	Pointers.GameFrameWork.SendNetMsgZoneMgr = Memory::FindPatternIDA("55 8B EC 8B 45 0C 56 8B F1 8B 4D 08 C1 E1 0A 25 FF 03 00 00 0B C1 8B 8E 58 09 00 00 6A 02 8D 55 0C 52 66 89 45 0C E8 ? ? ? ? 8B 45 14 8B 4D 10 50 51 8B 8E 58 09 00 00 E8 ? ? ? ? 5E 5D C2 10 00");
	Pointers.GameFrameWork.SendNetMsgWorldMgr = Memory::FindPatternIDA("55 8B EC 8B 45 0C 56 8B F1 8B 4D 08 C1 E1 0A 25 FF 03 00 00 0B C1 8B 8E 5C 09 00 00 6A 02 8D 55 0C 52 66 89 45 0C");
		
	// MainApp
	Pointers.MainApp.GetMainAppInstance = Memory::FindPatternIDA("A1 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC 55 8B EC 83 EC 10 A1");

	// NiApplication
	Pointers.NiApplication.WinProc = Memory::FindPatternIDA("55 8B EC 8B 0D ? ? ? ? 83 EC 10");

	// Timer
	Pointers.Timer.ms_fCurTime = *(UINT32*) (Memory::FindPatternIDA("D9 05 ? ? ? ? D9 86 08 02 00 00 DC 05 ? ? ? ? DC 25") + 2);
	
	// Patches
	Pointers.Patches.DisableDeprecatedFiles = Memory::FindPatternIDA("0F 84 ? ? ? ? 8B 85 DC FC FF FF 50 E8");
	Pointers.Patches.AntiCheatKill = Memory::FindPatternIDA("55 8B EC 83 EC 28 A1 ? ? ? ? 33 C5 89 45 FC 53 56 57 E8");
	
	// checks
	if (!Pointers.Anticheat.LoadXignCode)
	{
		Log::WriteLine("failed - code 0x0", ConsoleColor::RED);
	}
	else if (!Pointers.GameFrameWork.Constructor)
	{
		Log::WriteLine("failed - code 0x1", ConsoleColor::RED);
	}
	else if (!Pointers.GameFrameWork.ProcessNetMsg)
	{
		Log::WriteLine("failed - code 0x2", ConsoleColor::RED);
	}
	else if (!Pointers.GameFrameWork.SendNetMsgZoneMgr)
	{
		Log::WriteLine("failed - code 0x3", ConsoleColor::RED);
	}
	else if (!Pointers.MainApp.GetMainAppInstance)
	{
		Log::WriteLine("failed - code 0x4", ConsoleColor::RED);
	}
	else if (!Pointers.GameFrameWork.InitWindow)
	{
		Log::WriteLine("failed - code 0x5", ConsoleColor::RED);
	}
	else if (!Pointers.GameFrameWork.TerminateWindow)
	{
		Log::WriteLine("failed - code 0x6", ConsoleColor::RED);
	}
	else if (!Pointers.Patches.DisableDeprecatedFiles)
	{
		Log::WriteLine("failed - code 0x7", ConsoleColor::RED);
	}
	else if (!Pointers.GameFrameWork.SendNetMsgWorldMgr)
	{
		Log::WriteLine("failed - code 0x8", ConsoleColor::RED);
	}
	else if (!Pointers.NiApplication.WinProc)
	{
		Log::WriteLine("failed - code 0x9", ConsoleColor::RED);
	}
	else if (!Pointers.Patches.AntiCheatKill)
	{
		Log::WriteLine("failed - code 0xA", ConsoleColor::RED);
	}
	else
	{
		Log::WriteLine("success!", ConsoleColor::GREEN);
		Pointers.IsSuccesful = true;
	}
}
