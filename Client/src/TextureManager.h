#pragma once
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "json/json.h"
#include "Texture.h"

class TextureManager final
{
public:
	static TextureManager* GetInstance() { static TextureManager instance; return &instance; }

	void LoadTexture(const std::string& texturePath)
	{
		if (m_TextureCache.find(texturePath) != m_TextureCache.end())
		{
			return;
		}
		m_TextureCache[texturePath] = new Texture(texturePath);
	}

	void LoadTextureAtlas(const std::string& jsonFilePath, const std::string& filePath)
	{
		std::ifstream jsonFile(jsonFilePath);
		Json::CharReaderBuilder builder;
		builder["collectComments"] = false;
		Json::Value value;

		JSONCPP_STRING errs;
		bool bIsOk = parseFromStream(builder, jsonFile, &value, &errs);
		if (!bIsOk)
			assert(false && "Json File�� �Ľ��ϴµ� �����Ͽ����ϴ�.");


		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		if (!data)
		{
			assert(false && "�ؽ��ĸ� �ε��ϴµ� �����Ͽ����ϴ�.");
		}

		for (int i = 0; i < value.size(); i++)
		{
			m_TextureCache[value[i]["name"].asString()] = new Texture(data, value[i]["frame"]["x"].asInt(), value[i]["frame"]["y"].asInt(),
				value[i]["frame"]["w"].asInt(), value[i]["frame"]["h"].asInt());
		}

		stbi_image_free(data);
	}

	Texture* GetTexture(const std::string& alias)
	{
		if (m_TextureCache.find(alias) == m_TextureCache.end())
			assert(false && "�ؽ��ĸ� ã�� �� �����ϴ�. ���� �ε� ���ּ���");
		return m_TextureCache[alias];
	}


private:
	TextureManager() = default;
	~TextureManager() = default;

private:
	std::unordered_map<std::string, Texture*> m_TextureCache;
};
