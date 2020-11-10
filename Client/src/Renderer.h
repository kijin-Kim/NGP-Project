#pragma once
#include "Shader.h"
#include <glm/glm.hpp>

class Renderer final
{
public:
	struct Quad
	{
		glm::vec3 Position = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 Color = glm::vec3(0.0f, 0.0f, 0.0f);
	};


	Renderer()
	{
		m_Shader = new Shader("assets/shaders/VertexShader.glsl", "assets/shaders/FragmentShader.glsl");
		const float quadVertices[] = {
			-0.5f,  0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f
		};
		const unsigned int quadIndices[] = {
			0, 2, 1,
			0, 3, 2
		};


		glGenBuffers(2, m_BufferIDs);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferIDs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferIDs[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



		glGenVertexArrays(1, &m_VertexArrayID);


		glBindVertexArray(m_VertexArrayID);


		glBindBuffer(GL_ARRAY_BUFFER, m_BufferIDs[0]);
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferIDs[1]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);


	}
	~Renderer()
	{
		delete m_Shader;
	}

	void DrawQuad(const Quad& quad)
	{

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

		m_Shader->Bind();
		glm::mat4x4 modelMat = glm::mat4x4(1.0f);
		modelMat = glm::scale(modelMat, glm::vec3(10.0f, 1.0f, 1.0f));
		modelMat = glm::translate(modelMat, quad.Position);
		glm::mat4x4 projMat = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, 0.1f,  100.0f);
		m_Shader->SetMat4("u_Model", modelMat);
		m_Shader->SetMat4("u_Proj", projMat);

		glBindVertexArray(m_VertexArrayID);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

private:
	unsigned int m_VertexArrayID;
	unsigned int m_BufferIDs[2];
	Shader* m_Shader;
};
