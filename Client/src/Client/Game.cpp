#include "Game.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <assert.h>
#include "TextureManager.h"
#include "State.h"

#include <iostream>
#include <clocale>
#include "Network/Network.h"

Game::Game(int width, int height) :
	m_Width(width),
	m_Height(height)
{
	std::locale::global(std::locale(""));
	/* Initialize the library */
	int result = glfwInit();
	assert(result && "GLFW를 초기화 하는데 실패하였습니다.");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSwapInterval(1);

	/* Create a windowed mode window and its OpenGL context */

	m_Window = glfwCreateWindow(m_Width, m_Height, "Pickachu VolleyBall", NULL, NULL);
	assert(m_Window && "GLFW Window를 생성하는데 실패하였습니다.");

	/* Make the window's context current */
	glfwMakeContextCurrent(m_Window);

	glfwSetWindowUserPointer(m_Window, &m_UserPointer);
	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			UserInput input = {};
			input.Key = key;
			input.Action = action;

			UserPointer* userPointer = (UserPointer*)glfwGetWindowUserPointer(window);
			userPointer->m_InputQueue.push(input);
		});

	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int c)
		{
			wchar_t wc = (wchar_t)c;
			UserPointer* userPointer = (UserPointer*)glfwGetWindowUserPointer(window);
			userPointer->m_CharQueue.push(wc);
		});

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		assert(false);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	m_Renderer = new Renderer(m_Width, m_Height);

	ConnectToServer();



}

Game::~Game()
{
	delete m_Renderer;
	delete m_State;
	glfwTerminate();
}

void Game::ConnectToServer()
{
	Network* network = Network::GetInstance();
	network->isServer = false;
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);

	//int option = TRUE;
	//setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));

	network->Connect(m_Socket, "127.0.0.1");
}

void Game::Run()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		m_Renderer->Clear();
		/* Render here */
		if (m_State)
		{
			m_State->SendData();
			m_State->ReceiveData();
			m_State->Render();
		}
		/* Swap front and back buffers */
		glfwSwapBuffers(m_Window);
		/* Poll for and process events */
		glfwPollEvents();
	}
}

State* Game::GetState() const
{
	return m_State;
}

void Game::SetState(State* state)
{
	m_State = state;
}