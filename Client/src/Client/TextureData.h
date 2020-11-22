#pragma once
#include <string>
#include <GL/glew.h>
#include <assert.h>
#include "stb/stb_image.h"
#include <unordered_map>


class TextureData
{
public:
	TextureData(const std::string& filePath)
	{
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &nrChannels, 0);
		if (!data)
		{
			assert(false && "텍스쳐를 로드하는데 실패하였습니다.");
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		
	}


	~TextureData()
	{
		glDeleteTextures(1, &m_ID);
	}

	void Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}


	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

private:
	unsigned int m_ID;
	int m_Width = 0;
	int m_Height = 0;
};

struct TextureAtlasData
{
	TextureData* Data;
	std::unordered_map<std::string, int[6]> AtlasDataMap;
};

struct Texture
{
	TextureData* Data = nullptr;
	int X = 0;
	int Y = 0;
	int W = 0;
	int H = 0;
	int IW = 0;
	int IH = 0;
};