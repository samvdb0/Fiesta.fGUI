#include "pch.h"
#include "Menu.h"

#include <d3d9.h>
#include <detours.h>

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "ImGuiFileDialog.h"

#include "IconsFontAwesome5.h"

#include "Memory.h"
#include "Config.h"
#include "MainApp.h"

bool Menu::Initialized;

bool Menu::Show;

bool Menu::ShowTestWindow;
bool Menu::ShowAboutWindow;

bool Menu::IsListeningForHotkey;

WNDPROC WndProc_Org;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcH(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{	
	if (uMsg == WM_KEYUP && wParam == Data::MenuHotkey)
	{
		Menu::Show = !Menu::Show;
		MainApp::GetMainAppInstance()->m_bEnableInput = !Menu::Show;
	}
	
	if (uMsg == WM_KEYUP)
	{
		if (Menu::IsListeningForHotkey)
		{
			Data::MenuHotkey = wParam;
			Menu::IsListeningForHotkey = false;
		}
	}
	
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	
	return WndProc_Org(hWnd, uMsg, wParam, lParam);
}

HRESULT(__stdcall* EndScene_Org)(LPDIRECT3DDEVICE9);
HRESULT __stdcall EndSceneH(LPDIRECT3DDEVICE9 device)
{
	if (!Menu::Initialized)
	{
		D3DDEVICE_CREATION_PARAMETERS d3dpm;
		ZeroMemory(&d3dpm, sizeof d3dpm);
		device->GetCreationParameters(&d3dpm);

		Menu::Setup(d3dpm.hFocusWindow, device);
		Menu::Initialized = true;
	}

	if (Menu::Show)
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
		Menu::Render();
		
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	return EndScene_Org(device);
}

HRESULT(__stdcall* Reset_Org)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
HRESULT __stdcall ResetH(const LPDIRECT3DDEVICE9 device, D3DPRESENT_PARAMETERS* params)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	
	auto ret = Reset_Org(device, params);

	ImGui_ImplDX9_CreateDeviceObjects();

	return ret;
}

void Menu::Install()
{
	EndScene_Org = (HRESULT(__stdcall*)(LPDIRECT3DDEVICE9)) Detour(Memory::D3DVTable(42), EndSceneH);
	Reset_Org = (HRESULT(__stdcall*)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*)) Detour(Memory::D3DVTable(16), ResetH);
	WndProc_Org = (WNDPROC) Detour(Data::Pointers.NiApplication.WinProc, WndProcH);
}

void Menu::Setup(const HWND hwnd, const LPDIRECT3DDEVICE9 device)
{	
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	// io.Fonts->AddFontDefault();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.Fonts->AddFontFromFileTTF("./resutil/fonts/ruda.ttf", 15.0f);
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	
	// merge in icons from Font Awesome
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF("./resutil/fonts/" FONT_ICON_FILE_NAME_FAS, 13.0f, &icons_config, icons_ranges);
	
	ImGui::StyleColorsDark();

	auto* style = &ImGui::GetStyle();
	style->WindowRounding = 4.0f;
	style->FrameRounding = 3.0f;
	style->IndentSpacing = 12.5f;
	style->ScrollbarSize = 12.0f;
	style->ScrollbarRounding = 6.0f;
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(device);
}

void Menu::Render()
{
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::MenuItem(ICON_FA_DRAGON " " PROJ_NAME);
		
		if (ImGui::BeginMenu(ICON_FA_STREAM " Features"))
		{
			if (ImGui::MenuItem(ICON_FA_QUESTION_CIRCLE " Test Window"/*,  "F5", &ShowQuestWindow*/))
			{
				ShowTestWindow = !ShowTestWindow;
			}
						
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(ICON_FA_COG " Config"))
		{
			if (ImGui::BeginMenu("Default (autoloaded)"))
			{
				if (ImGui::MenuItem("Load"))
				{
					Config::Load(CFG_PATH_DEFAULT);
				}

				if (ImGui::MenuItem("Save"))
				{
					Config::Save(CFG_PATH_DEFAULT);
				}
				
				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("Slot #1"))
			{
				if (ImGui::MenuItem("Load"))
				{
					Config::Load(CFG_PATH_ONE);
				}

				if (ImGui::MenuItem("Save"))
				{
					Config::Save(CFG_PATH_ONE);
				}
				
				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("Slot #2"))
			{
				if (ImGui::MenuItem("Load"))
				{
					Config::Load(CFG_PATH_TWO);
				}

				if (ImGui::MenuItem("Save"))
				{
					Config::Save(CFG_PATH_TWO);
				}
				
				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("Slot #3"))
			{
				if (ImGui::MenuItem("Load"))
				{
					Config::Load(CFG_PATH_THREE);
				}

				if (ImGui::MenuItem("Save"))
				{
					Config::Save(CFG_PATH_THREE);
				}
				
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem(ICON_FA_INFO_CIRCLE " About"))
		{
			ShowAboutWindow = !ShowAboutWindow;
		}
		
		ImGui::EndMainMenuBar();
	}
	
	if (ShowTestWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(650, 500));
		ImGui::Begin(ICON_FA_INDUSTRY " Test Window", &ShowTestWindow, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize);
		
		ImGui::Separator();
		ImGui::Text("Category");
		ImGui::Separator();
		
		ImGui::Text(ICON_FA_EXCLAMATION_TRIANGLE " A test window");
		
		ImGui::End();
	}
	
	if (ShowAboutWindow)
	{
		ImGui::Begin(ICON_FA_INFO_CIRCLE " About", &ShowAboutWindow, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Separator();
		ImGui::Text(ICON_FA_DRAGON " " PROJ_NAME);
		ImGui::Separator();
		
		ImGui::Text("Created with " ICON_FA_HEART " " PROJ_COPYRIGHT);
		ImGui::Text("Current version: " PROJ_VERSION);
	}
}
