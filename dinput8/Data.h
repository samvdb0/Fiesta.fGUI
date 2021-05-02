#pragma once
#include <Windows.h>

class Data
{
public:
	static void Initialize();
	
	static struct DATA_PTR_STRUCT
	{
		struct
		{
			DWORD LoadXignCode;
		} Anticheat;
		
		struct
		{
			DWORD Constructor;
			DWORD InitWindow;
			DWORD TerminateWindow;
			
			DWORD ProcessNetMsg;
			DWORD SendNetMsgZoneMgr;
			DWORD SendNetMsgWorldMgr;
		} GameFrameWork;
		
		struct
		{
			DWORD GetMainAppInstance;
		} MainApp;

		struct
		{
			DWORD WinProc;
		} NiApplication;
		
		struct
		{
			DWORD ms_fCurTime;
		} Timer;

		struct
		{
			DWORD DisableDeprecatedFiles;
			DWORD AntiCheatKill;
		} Patches;

		bool IsSuccesful;
	} Pointers;
	
	static struct DATA_ASM_INSTRUCTIONS
	{
		unsigned char Ret[1] = { 0xC3 };
		unsigned char Jmp[1] = { 0xEB };
		unsigned char Je[2] = { 0x0F, 0x84 };
		unsigned char Jne[2] = { 0x0F, 0x85 };
		unsigned char Nop1[1] = { 0x90 };
		unsigned char Nop2[2] = { 0x90, 0x90 };
		unsigned char Nop6[6] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	} Instructions;

	static struct DATA_CLASS_INSTANCES
	{
		class GameFrameWork* GameFrameWork;
	} Instances;

	static int MenuHotkey;

	static bool IsIngame;
};
