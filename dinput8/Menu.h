#pragma once
#include <d3d9.h>

class Menu
{
public:
	static void Install();
	static void Setup(HWND hwnd, LPDIRECT3DDEVICE9 device);

	static void Render();

	static bool Initialized;
	static bool Show;
	
	static bool ShowTestWindow;
	static bool ShowAboutWindow;

	static bool IsListeningForHotkey;
};

