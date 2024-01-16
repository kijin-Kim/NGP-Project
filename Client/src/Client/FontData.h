#pragma once
#include <string>
#include <ft2build.h>
#include <assert.h>
#include <unordered_map>
#include "GL\glew.h"
#include FT_FREETYPE_H  

struct Font
{
	unsigned int FontID = 0;
	int Left = 0;
	int Top = 0;
	int Width = 0;
	int Row = 0;
	int Advance = 0;
};

class FontData final
{
public:
	FontData(const std::string& filePath, unsigned int size)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			assert(false && "FreeType 라이브러리를 불러오는데 실패하였습니다.");
		}

		FT_Face face;
		if (FT_New_Face(ft, filePath.c_str(), 0, &face))
		{
			assert(false && "Font를 불러오는데 실패하였습니다.");
		}
		FT_Set_Pixel_Sizes(face, 0, size);

		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				assert(false && "Glyph를 불러오는데 실패하였습니다.");
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use

			Font font = {};
			font.FontID = texture;
			font.Left = face->glyph->bitmap_left;
			font.Top = face->glyph->bitmap_top;
			font.Width = face->glyph->bitmap.width;
			font.Row = face->glyph->bitmap.rows;
			font.Advance = face->glyph->advance.x >> 6;

			m_FontCache.insert(std::pair(c, font));

			
		}
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	~FontData()
	{
		for (auto& font : m_FontCache)
			glDeleteTextures(1, &font.second.FontID);
	}

	const Font& GetFont(const unsigned char c)
	{
		auto it = m_FontCache.find(c);
		if (it == m_FontCache.end())
			assert(false && "Character를 찾을 수 없습니다.");

		return (*it).second;
	}

private:
	std::unordered_map<unsigned char, Font> m_FontCache;
	
};