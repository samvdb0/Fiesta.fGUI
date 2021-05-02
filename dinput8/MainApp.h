#pragma once
class MainApp
{
public:
	static MainApp* GetMainAppInstance();

	char _pad[0x2A0];
	bool m_bEnableInput;
};

