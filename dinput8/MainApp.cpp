#include "pch.h"
#include "MainApp.h"

MainApp* MainApp::GetMainAppInstance()
{
	return ((MainApp* (__cdecl*)()) Data::Pointers.MainApp.GetMainAppInstance)();
}
