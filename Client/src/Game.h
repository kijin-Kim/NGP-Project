#pragma once
#include <queue>
#include "TextureData.h"
#include "Network/Data.h"

class Game final
{
public:
	Game(int width, int height);
	~Game();

	bool ConnectToServer() {}

	void ProcessInput();
	void SendDataToServer() {}
	void RecieveDataFromServer() {}
	void Run();


private:
	struct GLFWwindow* m_Window = nullptr;
	class Renderer* m_Renderer = nullptr;
	int m_Width;
	int m_Height;

	std::queue<UserInput> m_InputQueue;

	Texture testTexture = {};
};