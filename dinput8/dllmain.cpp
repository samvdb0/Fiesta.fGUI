#include "pch.h"
#include <dinput.h>

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "d3d9.lib")

#include "Config.h"
#include "Memory.h"
#include "Data.h"
#include "GameFrameWork.h"
#include "Log.h"
#include "Menu.h"

HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter)
{	
    char libPath[MAX_PATH] = { 0 };
    GetSystemDirectoryA(libPath, sizeof libPath);
    strcat_s(libPath, MAX_PATH, "\\dinput8.dll");
    auto lib = LoadLibraryA(libPath);
     
    if (lib)
    {
	    auto function = GetProcAddress(lib, "DirectInput8Create");
		if (function)
		{
			return ((HRESULT(WINAPI*)(HINSTANCE,DWORD,REFIID,LPVOID *,LPUNKNOWN)) function)(hinst, dwVersion, riidltf, ppvOut, punkOuter);
		}
    }

	return E_FAIL;
}

bool AuthorityPatched = false;

DWORD WINAPI InstallMenuAsync(LPVOID lpParam)
{
	Log::Write("?", "Loading menu... ");
	Menu::Install();
	Log::WriteLine("success!", ConsoleColor::GREEN);
	Log::WriteLine("!", "Press INSERT once ingame to open/close the menu.");
	return 0;
}

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
		AllocConsole();
		freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
		SetConsoleTitleA(PROJ_NAME " " PROJ_VERSION " - " PROJ_COPYRIGHT);
    	
		Data::Initialize();

    	if (!Data::Pointers.IsSuccesful)
    	{
			Log::WaitInput();
			std::exit(0);
    	}

		Log::Write("?", "Disabling XIGNCODE... ");
		if (!Memory::WriteBytes(Data::Pointers.Anticheat.LoadXignCode, Data::Instructions.Ret, sizeof Data::Instructions.Ret))
		{
			Log::WriteLine("failed", ConsoleColor::RED);
			Log::WaitInput();
			std::exit(0);
		}
		Log::WriteLine("success!", ConsoleColor::GREEN);
    	
		Log::Write("?", "Disabling internal anticheat... ");
		if (!Memory::WriteBytes(Data::Pointers.Patches.AntiCheatKill, Data::Instructions.Ret, sizeof Data::Instructions.Ret))		{
			Log::WriteLine("failed", ConsoleColor::RED);
			Log::WaitInput();
			std::exit(0);
		}
		Log::WriteLine("success!", ConsoleColor::GREEN);

		Log::Write("?", "Disabling DeprecatedFiles.shn-related functions... ");
		if (!Memory::WriteBytes(Data::Pointers.Patches.DisableDeprecatedFiles, Data::Instructions.Jne, sizeof Data::Instructions.Jne))
		{
			Log::WriteLine("failed", ConsoleColor::RED);
			Log::WaitInput();
			std::exit(0);
		}
		Log::WriteLine("success!", ConsoleColor::GREEN);
    	
		Log::Write("?", "Installing features... ");
		GameFrameWork::Install();
		Log::WriteLine("success!", ConsoleColor::GREEN);
		
		Config::Load(CFG_PATH_DEFAULT);
		
		CreateThread(nullptr, 0, &InstallMenuAsync, nullptr, 0, nullptr);
		DisableThreadLibraryCalls(hModule);
    }
	
	return TRUE;
}