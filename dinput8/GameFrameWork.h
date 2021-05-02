#pragma once
class GameFrameWork
{
public:
	__declspec(noinline) static void Install();
	
	void __fastcall Constructor();
	bool InitWindow();
	void TerminateWindow();
	
	void SendNetMsgZoneMgr(unsigned short department, unsigned short command, void* proto, int len);
	void SendNetMsgWorldMgr(unsigned short department, unsigned short command, void* proto, int len);
};
