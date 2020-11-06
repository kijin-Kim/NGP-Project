#pragma once
#include "Shader.h"

class Renderer final
{
public:
	Renderer()
	{
		m_Shader = new Shader("assets/shaders/VertexShader.glsl", "assets/shaders/FragmentShader.glsl");
		static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
		};

		
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenVertexArrays(1, &vaoID);


		glBindVertexArray(vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		// ������ ù��° �Ӽ���(attribute) : ���ؽ���
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // 0��° �Ӽ�(attribute). 0 �� �� Ư���� ������ ������, ���̴��� ���̾ƿ�(layout)�� �ݵ�� ���߾�� �մϴ�.
			3,                  // ũ��(size)
			GL_FLOAT,           // Ÿ��(type)
			GL_FALSE,           // ����ȭ(normalized)?
			0,                  // ���� ��� ���� ����(stride)
			(void*)0            // �迭 ������ ������(offset; �ű�� ��)
		);
		// �ﰢ�� �׸���!

		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glBindVertexArray(0);


	}
	~Renderer()
	{
		delete m_Shader;
	}


	void Draw()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

		m_Shader->Bind();

		glBindVertexArray(vaoID);
		
		glDrawArrays(GL_TRIANGLES, 0, 3); // ���ؽ� 0���� �����ؼ�; �� 3���� ���ؽ��� -> �ϳ��� �ﰢ��
		
		glBindVertexArray(0);

	
	}

private:
	GLuint vaoID;
	GLuint vboID;
	Shader* m_Shader;
};
