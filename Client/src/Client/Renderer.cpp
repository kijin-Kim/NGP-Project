#include "Renderer.h"

Renderer::Renderer(int width, int height)
{
	m_Shader = new Shader("assets/shaders/VertexShader.glsl", "assets/shaders/FragmentShader.glsl");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	float quadVertices[12] = {
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	unsigned int quadIndices[6] = {
		0, 2, 1,
		0, 3, 2
	};

	float quadTexCoord[8] = {
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


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
}

Renderer::~Renderer()
{
	delete m_Shader;
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::DrawQuad(const Quad& quad)
{

	float quadVertices[12] = {
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	unsigned int quadIndices[6] = {
		0, 2, 1,
		0, 3, 2
	};

	float quadTexCoord[8] = {
		 0.0f, 1.0f,
		 1.0f, 1.0f,
		 1.0f, 0.0f,
		 0.0f, 0.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_BufferIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferIDs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_BufferIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoord), quadTexCoord, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	m_Shader->Bind();
	m_Shader->SetMat4("u_Proj", m_ProjMat);

	glm::mat4x4 modelMat = glm::mat4x4(1.0f);
	if(!quad.bUseFont)
		modelMat = glm::translate(modelMat, glm::vec3(quad.Position, 0.0f));
	else
		modelMat = glm::translate(modelMat, glm::vec3(quad.Position.x + quad.Font.Width / 2.0f, quad.Position.y + quad.Font.Row / 2.0f - (quad.Font.Row - quad.Font.Top), 0.0f));

	Texture image = quad.Image;
	if (quad.bUseTexture && image.Data)
	{
		m_Shader->SetInt1("u_bToggleTexture", 1);

		image.Data->Bind(0);
		modelMat = glm::scale(modelMat, glm::vec3(image.W, image.H, 1.0f));

		if (image.X != 0 || image.Y != 0)
		{
			if (!quad.bShouldFlipVertical)
			{
				float quadTexCoord[8] = {
				image.X / (float)image.IW, image.Y / (float)image.IH,
				(image.X + image.W) / (float)image.IW, image.Y / (float)image.IH,
				(image.X + image.W) / (float)image.IW, (image.Y - image.H) / (float)image.IH,
				image.X / (float)image.IW, (image.Y - image.H) / (float)image.IH
				};
				glBindBuffer(GL_ARRAY_BUFFER, m_BufferIDs[2]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoord), quadTexCoord, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else
			{
				float quadTexCoord[8] = {
				(image.X + image.W) / (float)image.IW,image.Y / (float)image.IH,
				image.X / (float)image.IW, image.Y / (float)image.IH,
				image.X / (float)image.IW, (image.Y - image.H) / (float)image.IH,
				(image.X + image.W) / (float)image.IW,(image.Y - image.H) / (float)image.IH
				};
				glBindBuffer(GL_ARRAY_BUFFER, m_BufferIDs[2]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoord), quadTexCoord, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		m_Shader->SetInt1("u_Texture", 0);
	}
	else
	{
		m_Shader->SetInt1("u_bToggleTexture", 0);
	}

	if (quad.bUseFont)
	{
		m_Shader->SetInt1("u_bToggleFont", 1);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, quad.Font.FontID);


		glBindBuffer(GL_ARRAY_BUFFER, m_BufferIDs[2]);

		float quadTexCoord[8] = {
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f,
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoord), quadTexCoord, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		modelMat = glm::scale(modelMat, glm::vec3(quad.Font.Width, quad.Font.Row, 1.0f));
		m_Shader->SetInt1("u_Texture", 0);
	}
	else
		m_Shader->SetInt1("u_bToggleFont", 0);

	

	if (quad.bUseColor)
	{
		m_Shader->SetInt1("u_bToggleColor", 1);
		m_Shader->SetVec4("u_Color", quad.Color);

		if(!quad.bUseTexture && !quad.bUseFont)
			modelMat = glm::scale(modelMat, glm::vec3(quad.Size.x, quad.Size.y, 1.0f));
	}
	else
		m_Shader->SetInt1("u_bToggleColor", 0);



	m_Shader->SetMat4("u_Model", modelMat);


	//else
	//{
	//	// NO IMAGE
	//	modelMat = glm::scale(modelMat, glm::vec3(quad.Size.x, quad.Size.y, 1.0f));

	//}



	glBindVertexArray(m_VertexArrayID);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	m_Shader->UnBind();
}