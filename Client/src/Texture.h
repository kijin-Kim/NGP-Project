#pragma once
#include <string>
#include <GL/glew.h>
#include <assert.h>
#include "stb/stb_image.h"


class Texture
{
public:
	Texture(const std::string& filePath, int subWidth = 0, int subHeight = 0)
	{
		unsigned int m_ID;
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		if (!data)
		{
			assert(false && "텍스쳐를 로드하는데 실패하였습니다.");
		}
		
		if(subWidth == 0 || subHeight == 0)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, subWidth, subHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		stbi_image_free(data);
	}

	~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}

	void Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

private:
	unsigned int m_ID;

};
