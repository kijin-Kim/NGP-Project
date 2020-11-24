#include "Game.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <assert.h>
#include "Renderer.h"

Game::Game(int width, int height) :
	m_Width(width),
	m_Height(height)
{
	/* Initialize the library */
	int result = glfwInit();
	assert(result && "GLFW�� �ʱ�ȭ �ϴµ� �����Ͽ����ϴ�.");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSwapInterval(1);

	/* Create a windowed mode window and its OpenGL context */

	m_Window = glfwCreateWindow(m_Width, m_Height, "Hello World", NULL, NULL);
	assert(m_Window && "GLFW Window�� �����ϴµ� �����Ͽ����ϴ�.");


	/* Make the window's context current */
	glfwMakeContextCurrent(m_Window);

	
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		assert(false);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	m_Renderer = new Renderer();
}

Game::~Game()
{
	delete m_Renderer;
	glfwTerminate();
}

void Game::ProcessInput()
{
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window, true);
}

void Game::Run()
{
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_Window))
	{
		ProcessInput();
		/* Render here */
		m_Renderer->Draw();
		/* Swap front and back buffers */
		glfwSwapBuffers(m_Window);
		/* Poll for and process events */
		glfwPollEvents();

	}
}