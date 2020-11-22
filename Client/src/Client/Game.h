#pragma once
#include <queue>
#include "TextureData.h"
#include "Network/Data.h"
#include "State.h"

class Game
{
public:
	Game(int width, int height);
	virtual ~Game();

	bool ConnectToServer() {}
	void ProcessInput();
	void SendDataToServer() {}
	void RecieveDataFromServer() {}
	void Run();

protected:
	State* GetState() const { return m_State; }
	void SetState(State* state) { m_State = state; }

	
private:
	struct GLFWwindow* m_Window = nullptr;
	class Renderer* m_Renderer = nullptr;
	int m_Width;
	int m_Height;

	std::queue<UserInput> m_InputQueue;

	State* m_State = nullptr;
};

Game* CreateGameApplication();

#define RegisterApplication(x, width, height) Game* CreateGameApplication() { return new x(width, height); }