#include "pch.h"
#include "GameFrameWork.h"

void(__thiscall* Constructor_Org)(GameFrameWork*);
bool(__thiscall* InitWindow_Org)(GameFrameWork*);
void(__thiscall* TerminateWindow_Org)(GameFrameWork*);

void GameFrameWork::Install()
{
	Constructor_Org = (void(__thiscall*)(GameFrameWork*)) Detour(Data::Pointers.GameFrameWork.Constructor, MF(Constructor));

	InitWindow_Org = (bool(__thiscall*)(GameFrameWork*)) Detour(Data::Pointers.GameFrameWork.InitWindow, MF(InitWindow));
	TerminateWindow_Org = (void(__thiscall*)(GameFrameWork*)) Detour(Data::Pointers.GameFrameWork.TerminateWindow, MF(TerminateWindow));
}

void GameFrameWork::Constructor()
{
	Data::Instances.GameFrameWork = this;

	Constructor_Org(this);
}

bool GameFrameWork::InitWindow()
{
	Data::IsIngame = true;

	return InitWindow_Org(this);
}

void GameFrameWork::TerminateWindow()
{
	Data::IsIngame = false;

	return TerminateWindow_Org(this);
}

void GameFrameWork::SendNetMsgZoneMgr(const unsigned short department, const unsigned short command, void* proto, const int len)
{
	CallMemberFunction<GameFrameWork, void, unsigned short, unsigned short, void*, int>(Data::Pointers.GameFrameWork.SendNetMsgZoneMgr, this, department, command, proto, len);
}

void GameFrameWork::SendNetMsgWorldMgr(const unsigned short department, const unsigned short command, void* proto, const int len)
{
	CallMemberFunction<GameFrameWork, void, unsigned short, unsigned short, void*, int>(Data::Pointers.GameFrameWork.SendNetMsgWorldMgr, this, department, command, proto, len);
}
