#pragma once
#include <string>

class Memory
{
public:
	__declspec(noinline) static DWORD FindPatternIDA(const std::string& pattern);
	__declspec(noinline) static BOOL WriteBytes(DWORD dwAddress, LPBYTE lpBuffer, DWORD nBytes);
	__declspec(noinline) static DWORD D3DVTable(int index);
};
