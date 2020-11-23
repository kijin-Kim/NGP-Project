#pragma once
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "json/json.h"
#include "TextureData.h"



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
		m_TextureCache[texturePath] = new TextureData(texturePath);
	}



	void LoadTextureAtlas(const std::string& jsonFilePath, const std::string& filePath)
	{
		if (m_TextureAtlasCache.find(filePath) != m_TextureAtlasCache.end())
		{
			return;
		}
		m_TextureAtlasCache[filePath] = new TextureAtlasData();
		m_TextureAtlasCache[filePath]->Data = new TextureData(filePath);
		

		

		std::ifstream jsonFile(jsonFilePath);
		Json::CharReaderBuilder builder;
		builder["collectComments"] = false;
		Json::Value value;

		JSONCPP_STRING errs;
		bool bIsOk = parseFromStream(builder, jsonFile, &value, &errs);
		if (!bIsOk)
			assert(false && "Json File을 파싱하는데 실패하였습니다.");


		for (int i = 0; i < value.size(); i++)
		{
			const std::string textureName = value[i]["name"].asString();
			const unsigned int textureX = value[i]["frame"]["x"].asInt();
			const unsigned int textureY = value[i]["frame"]["y"].asInt();
			const unsigned int textureW = value[i]["frame"]["w"].asInt();
			const unsigned int textureH = value[i]["frame"]["h"].asInt();


			
			m_TextureAtlasCache[filePath]->AtlasDataMap[textureName][0] = textureX;
			m_TextureAtlasCache[filePath]->AtlasDataMap[textureName][1] = m_TextureAtlasCache[filePath]->Data->GetHeight() - textureY;
			m_TextureAtlasCache[filePath]->AtlasDataMap[textureName][2] = textureW;
			m_TextureAtlasCache[filePath]->AtlasDataMap[textureName][3] = textureH;
			m_TextureAtlasCache[filePath]->AtlasDataMap[textureName][4] = m_TextureAtlasCache[filePath]->Data->GetWidth();
			m_TextureAtlasCache[filePath]->AtlasDataMap[textureName][5] = m_TextureAtlasCache[filePath]->Data->GetHeight();

		}
	
	}

	Texture GetTexture(const std::string& alias)
	{
		Texture texture = {};

		if (m_TextureCache.find(alias) == m_TextureCache.end())
			assert(false && "텍스쳐를 찾을 수 없습니다. 먼저 로드 해주세요");

		texture.Data = m_TextureCache[alias];

		return texture;
	}

	Texture GetTextureFromAtlas(const std::string& textureAtlasName, const std::string& textureName)
	{
		Texture texture = {};

		if (m_TextureAtlasCache.find(textureAtlasName) == m_TextureAtlasCache.end())
			assert(false && "텍스쳐 아틀라스를 찾을 수 없습니다. 먼저 로드 해주세요");

		TextureAtlasData* texAtalsData = m_TextureAtlasCache[textureAtlasName];

		if (texAtalsData->AtlasDataMap.find(textureName) == texAtalsData->AtlasDataMap.end())
			assert(false && "텍스쳐 아틀라스에서 텍스쳐를 찾을 수 없습니다. 텍스쳐 아틀라스 Json File을 확인해주세요");

	

		texture.Data = texAtalsData->Data;
		texture.X = texAtalsData->AtlasDataMap[textureName][0];
		texture.Y = texAtalsData->AtlasDataMap[textureName][1];
		texture.W = texAtalsData->AtlasDataMap[textureName][2];
		texture.H = texAtalsData->AtlasDataMap[textureName][3];
		texture.IW = texAtalsData->AtlasDataMap[textureName][4];
		texture.IH = texAtalsData->AtlasDataMap[textureName][5];
	
		return texture;
	}


private:
	TextureManager() = default;
	~TextureManager()
	{
		for (auto texture : m_TextureCache)
			delete texture.second;
		for (auto texture : m_TextureAtlasCache)
			delete texture.second;
	}

private:
	std::unordered_map<std::string, TextureData*> m_TextureCache;
	std::unordered_map<std::string, TextureAtlasData*> m_TextureAtlasCache;
};
