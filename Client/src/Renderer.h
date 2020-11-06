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
		// 삼각형 그리기!

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
		
		glDrawArrays(GL_TRIANGLES, 0, 3); // 버텍스 0에서 시작해서; 총 3개의 버텍스로 -> 하나의 삼각형
		
		glBindVertexArray(0);

	
	}

private:
	GLuint vaoID;
	GLuint vboID;
	Shader* m_Shader;
};
