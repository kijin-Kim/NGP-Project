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

	bool ConnectToServer() {}
	void Run();


	std::queue<UserInput>& GetInputQueue() { return m_InputQueue; }

protected:
	State* GetState() const;
	void SetState(State* state);
	

	
private:
	struct GLFWwindow* m_Window = nullptr;
	class Renderer* m_Renderer = nullptr;
	int m_Width;
	int m_Height;

	std::queue<UserInput> m_InputQueue;

	State* m_State;
};

Game* CreateGameApplication();

#define RegisterApplication(x, width, height) Game* CreateGameApplication() { return new x(width, height); }