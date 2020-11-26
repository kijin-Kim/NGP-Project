#pragma once
#include "Game.h"
#include "Network/Network.h"
#include "Network/Data.h"

extern Game* CreateGameApplication(const std::string& serverIP);

#ifdef _DEBUG

int main(void)
{
	auto game = CreateGameApplication();
	game->Run();
	delete game;
}

#endif


#ifdef NDEBUG

#pragma comment(linker, "/subsystem:windows")
#include <windows.h>

int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	LPWSTR cmdLine = GetCommandLine();
	int argumentCount = 0;
	LPWSTR* argv = CommandLineToArgvW(cmdLine, &argumentCount);
	std::wstring wServerIP = argv[1];
	std::string serverIP;
	serverIP.assign(wServerIP.begin(), wServerIP.end());

	auto game = CreateGameApplication(serverIP);
	game->Run();
	delete game;
}

#endif



