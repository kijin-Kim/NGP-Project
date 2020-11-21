#pragma once
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>

class Renderer final
{
public:
	struct Quad
	{
		glm::vec2 Position = glm::vec2(0.0f, 0.0f);
		glm::vec2 Size = glm::vec2(1.0f, 1.0f);
		Texture* Image = nullptr;
		unsigned int RenderOrder = 0;
	};


	Renderer(int width, int height)
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

		const float quadTexCoord[] = {
			 0.0f, 1.0f,
			 1.0f, 1.0f,
			 1.0f, 0.0f,
			 0.0f, 0.0f
		};

		glGenBuffers(3, m_BufferIDs);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferIDs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferIDs[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_BufferIDs[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoord), quadTexCoord, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenVertexArrays(1, &m_VertexArrayID);


		glBindVertexArray(m_VertexArrayID);


		glBindBuffer(GL_ARRAY_BUFFER, m_BufferIDs[0]);
		// 버퍼의 첫번째 속성값(attribute) : 버텍스들
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // 0번째 속성(attribute). 0 이 될 특별한 이유는 없지만, 쉐이더의 레이아웃(layout)와 반드시 맞추어야 합니다.
			3,                  // 크기(size)
			GL_FLOAT,           // 타입(type)
			GL_FALSE,           // 정규화(normalized)?
			0,                  // 다음 요소 까지 간격(stride)
			(void*)0            // 배열 버퍼의 오프셋(offset; 옮기는 값)
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferIDs[1]);


		glBindBuffer(GL_ARRAY_BUFFER, m_BufferIDs[2]);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,                  
			2,                  
			GL_FLOAT,          
			GL_FALSE,          
			0,                  
			(void*)0
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glBindVertexArray(0);


		m_ProjMat = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);


	}
	~Renderer()
	{
		delete m_Shader;
	}

	void DrawQuad(const Quad& quad)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		m_Shader->Bind();
		m_Shader->SetMat4("u_Proj", m_ProjMat);


		glm::mat4x4 modelMat = glm::mat4x4(1.0f);
		float depth = quad.RenderOrder / 10.0f;
		modelMat = glm::translate(modelMat, glm::vec3(quad.Position, depth));
		modelMat = glm::scale(modelMat, glm::vec3(quad.Size, 1.0f));

		m_Shader->SetMat4("u_Model", modelMat);

		if (quad.Image)
		{
			quad.Image->Bind(0);
			m_Shader->SetInt1("u_Texture", 0);
		}

		glBindVertexArray(m_VertexArrayID);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		m_Shader->UnBind();
	}

private:
	glm::mat4x4 m_ProjMat = glm::mat4x4(1.0f);

	unsigned int m_VertexArrayID;
	unsigned int m_BufferIDs[3];
	Shader* m_Shader;
};
