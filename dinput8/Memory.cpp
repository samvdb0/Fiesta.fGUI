#include "pch.h"
#include "Memory.h"

#include <d3d9.h>
#include <Psapi.h>

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

DWORD Memory::FindPatternIDA(const std::string& pattern)
{
	auto pat = pattern.c_str();
	unsigned long firstMatch = 0;
	auto rangeStart = (DWORD) GetModuleHandle(nullptr);
	MODULEINFO miModInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	auto rangeEnd = rangeStart + miModInfo.SizeOfImage;
	
	for (auto pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
		{
			return firstMatch;
		}
		
		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
			{
				firstMatch = pCur;
			}
			
			if (!pat[2])
			{
				return firstMatch;
			}
			
			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
			{
				pat += 3;
			}
			else
			{
				pat += 2;
			}
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	
	return NULL;
}

BOOL Memory::WriteBytes(const DWORD dwAddress, const LPBYTE lpBuffer, const DWORD nBytes)
{
	if (nBytes == 0)
	{
		return FALSE;
	}

	DWORD dwOldProtect;

	if (!VirtualProtect((LPVOID) dwAddress, nBytes, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		return FALSE;
	}
	
	memcpy((LPVOID) dwAddress, lpBuffer, nBytes);

	if (!VirtualProtect((LPVOID) dwAddress, nBytes, dwOldProtect, &dwOldProtect))
	{
		return FALSE;
	}

	return TRUE;
}

LRESULT CALLBACK MsgProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }
DWORD Memory::D3DVTable(const int index)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, "DX", nullptr };
	RegisterClassEx(&wc);
	auto hWnd = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);
	
	auto pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	D3DPRESENT_PARAMETERS d3dpp { 0 };
	ZeroMemory(&d3dpp, sizeof d3dpp);
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	LPDIRECT3DDEVICE9 pd3dDevice;
	pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice);
	
	auto pVTable = *(DWORD**)pd3dDevice;
	auto dw = pVTable[index];
	
	if (pd3dDevice)
	{
		pd3dDevice->Release();
		pd3dDevice = nullptr;
	}

	DestroyWindow(hWnd);
	UnregisterClass("DX", GetModuleHandle(nullptr));
	
	return dw;
}

