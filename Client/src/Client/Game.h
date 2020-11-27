#pragma once
#include <queue>
#include "TextureData.h"
#include "Network/Data.h"
#include <WinSock2.h>
#include "Renderer.h"

class State;

class Game
{
public:
	Game(int width, int height, const std::string& serverIP);
	virtual ~Game();

	void Run();


	std::queue<UserInput>& GetInputQueue() { return m_UserPointer.m_InputQueue; }
	std::queue<wchar_t>& GetCharQueue() { return m_UserPointer.m_CharQueue; }

	SOCKET GetSocket() const { return m_Socket; }

	Renderer* GetRenderer() const { return m_Renderer; }

	unsigned int GetID() const { return m_ID; }
	void SetID(unsigned int id) { m_ID = id; }

	State* GetState() const;
	void SetGameState(State* state);

private:
	void ConnectToServer();
	
	

	
private:
	struct GLFWwindow* m_Window = nullptr;
	Renderer* m_Renderer = nullptr;
	int m_Width;
	int m_Height;

	struct UserPointer
	{
		std::queue<UserInput> m_InputQueue;
		std::queue<wchar_t> m_CharQueue;
	};

	UserPointer m_UserPointer;

	State* m_State;

	SOCKET m_Socket = {};


	unsigned int m_ID;

	std::string m_ServerIP;

};

Game* CreateGameApplication(const std::string& serverIP);

#define RegisterApplication(x, width, height) Game* CreateGameApplication(const std::string& serverIP) { return new x(width, height, serverIP); }