#pragma once
#include "Texture.h"
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

	Texture* testTexture = nullptr;
};