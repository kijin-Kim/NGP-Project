#pragma once
#include "Shader.h"
#include "TextureManager.h"
#include <glm/glm.hpp>



class Renderer final
{
public:
	struct Quad
	{
		glm::vec2 Position = glm::vec2(0.0f, 0.0f);

		bool bUseTexture = true;
		Texture Image = {};

		bool bUseColor = false;
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0, 1.0f);
		glm::vec2 Size = glm::vec2(1.0f, 1.0f);

		bool bShouldFlipVertical = false;
	};

	Renderer(int width, int height);
	~Renderer();


	static void RegisterQuads(Quad* quads, unsigned int quadCount);
	static void RegisterQuads(std::vector<Quad> quads);

	void Reset();
	void Draw();
	void DrawQuad(const Quad& quad);

private:
	glm::mat4x4 m_ProjMat = glm::mat4x4(1.0f);

	unsigned int m_VertexArrayID;
	unsigned int m_BufferIDs[3];
	Shader* m_Shader;

	static std::vector<Quad> m_Quads;
};

