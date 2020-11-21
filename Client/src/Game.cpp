#include "Game.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <assert.h>
#include "Renderer.h"
#include "TextureManager.h"

#include <iostream>

Game::Game(int width, int height) :
	m_Width(width),
	m_Height(height)
{
	/* Initialize the library */
	int result = glfwInit();
	assert(result && "GLFW를 초기화 하는데 실패하였습니다.");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSwapInterval(1);

	/* Create a windowed mode window and its OpenGL context */

	m_Window = glfwCreateWindow(m_Width, m_Height, "Hello World", NULL, NULL);
	assert(m_Window && "GLFW Window를 생성하는데 실패하였습니다.");


	/* Make the window's context current */
	glfwMakeContextCurrent(m_Window);


	glfwSetWindowUserPointer(m_Window, &m_InputQueue);
	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			std::queue<UserInput>* inputQueue = (std::queue<UserInput>*)glfwGetWindowUserPointer(window);
			UserInput input = {};
			input.Key = key;
			input.Action = action;
			inputQueue->push(input);
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

	TextureManager* textureManager = TextureManager::GetInstance();

	textureManager->LoadTexture("assets/textures/sprite_sheet.png");
	textureManager->LoadTextureAtlas("assets/textures/sprite_sheet.json", "assets/textures/sprite_sheet.png");

	testTexture = textureManager->GetTexture("ball/ball_0.png");
}

Game::~Game()
{
	delete m_Renderer;
	glfwTerminate();
}

void Game::ProcessInput()
{
	UserInput input = {};
	input.Key = GLFW_KEY_UNKNOWN;
	
	if (m_InputQueue.empty())
	{
		// Send Empty Input
	}
	else
	{
		input = m_InputQueue.front();

		// Send User Input

		m_InputQueue.pop();
	}
		
}

void Game::Run()
{
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_Window))
	{
		ProcessInput();
		/* Render here */
		Renderer::Quad quad = {};
		quad.Position = glm::vec3(320.0f, 240.0f, 0.0f);
		quad.Size = glm::vec2(100.0f, 100.0f);
		quad.Image = testTexture;
		m_Renderer->DrawQuad(quad);
		/* Swap front and back buffers */
		glfwSwapBuffers(m_Window);
		/* Poll for and process events */
		glfwPollEvents();

	}
}
