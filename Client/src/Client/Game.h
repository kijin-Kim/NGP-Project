#pragma once
#include <queue>
#include "TextureData.h"
#include "Network/Data.h"

class State;

class Game
{
public:
	Game(int width, int height);
	virtual ~Game();

	void Run();


	std::queue<UserInput>& GetInputQueue() { return m_UserPointer.m_InputQueue; }
	std::queue<wchar_t>& GetCharQueue() { return m_UserPointer.m_CharQueue; }


private:
	void ConnectToServer();
protected:
	State* GetState() const;
	void SetState(State* state);
	

	
private:
	struct GLFWwindow* m_Window = nullptr;
	class Renderer* m_Renderer = nullptr;
	int m_Width;
	int m_Height;

	struct UserPointer
	{
		std::queue<UserInput> m_InputQueue;
		std::queue<wchar_t> m_CharQueue;
	};

	UserPointer m_UserPointer;

	State* m_State;
};

Game* CreateGameApplication();

#define RegisterApplication(x, width, height) Game* CreateGameApplication() { return new x(width, height); }